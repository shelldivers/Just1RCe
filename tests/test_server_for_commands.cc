#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

int main(int argc, char *argv[])
{
    struct addrinfo hints, *result, *rp;
    int sfd, s;
    char buf[512];
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Prepare the socket address hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;      // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Bind to all interfaces

    // Get address info
    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    // Try to bind to one of the addresses
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        int opt = 1;
        if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt");
            close(sfd);
            continue;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    if (rp == NULL)
    {
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    // Listen for connections
    if (listen(sfd, 10) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %s...\n", argv[1]);

    for (;;)
    {
        // Accept a client connection
        peer_addr_len = sizeof(peer_addr);
        int connfd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (connfd == -1)
        {
            perror("accept");
            continue;
        }

        printf("Client connected\n");

        // Send welcome message
        const char *welcome = "001 ken Welcome to the networkname Network, ken!jiwojung@jhostname \r\n";
        // const char *welcome1 = ":Just1RCe 002 :Your host is Just1RCe, running version 1.0\r\n";
        // write(connfd, welcome, strlen(welcome1));

        // Handle client commands
        while ((nread = read(connfd, buf, sizeof(buf) - 1)) > 0)
        {
            buf[nread] = '\0';
            printf("Received: %s", buf);

            if (strncmp(buf, "NICK", 4) == 0)
            {
                // Handle NICK command
                write(connfd, welcome, strlen(welcome));
            }
            else if (strncmp(buf, "USER", 4) == 0)
            {
                // Handle USER command
                write(connfd, ":User registered\r\n", 29);
            }
            else if (strncmp(buf, "JOIN", 4) == 0)
            {
                std::string servername = "Just1RCe";
                std::string nickname = "ken";
                std::string channel = "channel";

                // JOIN 실패 메시지 작성
                std::string response = "471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n";

                // 클라이언트로 전송
                if (write(connfd, response.c_str(), response.size()) == -1) {
                    perror("Error writing JOIN failure response to socket");
                } else {
                    printf("Sent JOIN failure response: %s", response.c_str());
                }
            }
            else
            {
                // Default response for unknown commands
                write(connfd, ":server_name 400 user :Unknown command\r\n", 41);
            }
        }

        printf("Client disconnected\n");
        close(connfd);
    }

    close(sfd);
    return 0;
}
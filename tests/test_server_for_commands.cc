// g++ -o server_test -std=c++11 tests/test_server_for_commands.cc
// ./server_test 6667

// 컴파일 후, 서버를 6667로 열고 클라이언트를 열어
// /connect -nocap localhost 6667 
// 위의 명령어를 입력하여 테스트 서버에 접속 
// 후에 클라이언트에서 명령어를 입력하면 서버에 명령어가 어떤형식으로 들어오는지 알려준다   

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
        const char *welcome = ":irc.shelldivers.net 001 nickname :Welcome to the Internet Relay Network nickname!~username@172.30.0.1\r\n\
                               :irc.shelldivers.net 002 nickname :Your host is irc.shelldivers.net, running version ngircd-27 (x86_64/alpine/linux-musl)\r\n\
                               :irc.shelldivers.net 003 nickname :This server has been started Wed Feb 05 2025 at 17:05:47 (KST)\r\n\
                               :irc.shelldivers.net 004 nickname irc.shelldivers.net ngircd-27 abBcCFiIoqrRswx abehiIklmMnoOPqQrRstvVz\r\n\
                               :irc.shelldivers.net 005 nickname RFC2812 IRCD=ngIRCd CHARSET=UTF-8 CASEMAPPING=ascii PREFIX=(qaohv)~&@%+ CHANTYPES=#&+ CHANMODES=beI,k,l,imMnOPQRstVz CHANLIMIT=#&+:50 :are supported on this server\r\n\
                               :irc.shelldivers.net 005 nickname CHANNELLEN=50 NICKLEN=9 TOPICLEN=490 AWAYLEN=127 KICKLEN=400 MODES=5 MAXLIST=beI:50 EXCEPTS=e INVEX=I PENALTY FNC :are supported on this server\r\n";
        // Handle client commands
        while ((nread = read(connfd, buf, sizeof(buf) - 1)) > 0)
        {
            buf[nread] = '\0';
            printf("Received: %s", buf);

            if (strncmp(buf, "NICK", 4) == 0) {
                char new_nick[50] = {0};
                if (sscanf(buf, "NICK %49s", new_nick) == 1) {
                    char response[100] = {0};
                    snprintf(response, sizeof(response), ":nick_ken3 NICK :%s\r\n", new_nick);
                    
                    printf("[DEBUG] Sending: %s", response);  // 로그 추가
                    write(connfd, response, strlen(response));
                }
            }
            else if (strncmp(buf, "USER", 4) == 0)
            {
                // Handle USER command
                write(connfd, welcome, strlen(welcome));
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
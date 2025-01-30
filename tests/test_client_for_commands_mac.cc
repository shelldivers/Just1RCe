#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>

class IRCClient {
public:
    IRCClient(const std::string& ip, int port, const std::string& nickname)
        : ip_(ip), port_(port), nickname_(nickname), sockfd_(-1) {}

    ~IRCClient() {
        if (sockfd_ != -1) {
            close(sockfd_);
        }
    }

    void connectToServer() {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0) {
            throw std::runtime_error("Error creating socket");
        }

        struct sockaddr_in server_addr;
        std::memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);

        if (inet_pton(AF_INET, ip_.c_str(), &server_addr.sin_addr) <= 0) {
            throw std::runtime_error("Invalid IP address or address not supported");
        }

        if (connect(sockfd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            throw std::runtime_error("Connection failed");
        }

        std::cout << "Connected to " << ip_ << ":" << port_ << std::endl;
        sendCommand("NICK " + nickname_);
        sendCommand("USER " + nickname_ + " 0 * :" + nickname_);

        run();
    }

private:
    void sendCommand(const std::string& command) {
        std::string formatted_command = command + "\r\n";
        if (send(sockfd_, formatted_command.c_str(), formatted_command.size(), 0) < 0) {
            throw std::runtime_error("Failed to send command: " + command);
        }
    }

    void run() {
        char buffer[4096];

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                std::cout << "Disconnected from server." << std::endl;
                break;
            }
            std::cout << "[RECEIVED] " << buffer;

            // PING 처리
            if (std::strncmp(buffer, "PING", 4) == 0) {
                std::string pong_response = std::string(buffer).replace(0, 4, "PONG");
                std::cout << "[DEBUG] Responding to PING with: " << pong_response << std::endl;
                sendCommand(pong_response);
            }
        }
    }

    std::string ip_;
    int port_;
    std::string nickname_;
    int sockfd_;
};

int main() {
    try {
        std::string ip = "52.193.79.145";
        int port = 6667;  // 일반적으로 SSL을 사용하지 않는 포트

        std::string nickname;
        std::cout << "Enter nickname: ";
        std::cin >> nickname;
        std::cin.ignore();

        IRCClient client(ip, port, nickname);
        client.connectToServer();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

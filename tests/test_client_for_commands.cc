#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

class IRCClient {
public:
    IRCClient(const std::string& ip, int port)
        : ip_(ip), port_(port), sockfd_(-1) {}

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

        std::thread receive_thread([this]() { this->receiveMessages(); });
        receive_thread.detach();

        inputLoop();
    }

private:
    void sendCommand(const std::string& command) {
        std::string formatted_command = command + "\r\n";
        if (send(sockfd_, formatted_command.c_str(), formatted_command.size(), 0) < 0) {
            throw std::runtime_error("Failed to send command: " + command);
        }
    }

    void receiveMessages() {
        char buffer[4096];
        std::string nickname;

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                std::cout << "Disconnected from server." << std::endl;
                break;
            }
            std::string message(buffer);
            std::cout << "[RECEIVED]: " << message;

            if (message.find("PING") == 0) {
                std::string pong_response = message;
                pong_response.replace(0, 4, "PONG");
                std::cout << "[DEBUG] Responding to PING with: " << pong_response << std::endl;
                sendCommand(pong_response);
            }
            else if (message.find("PRIVMSG ") != std::string::npos && message.find(" :VERSION") != std::string::npos) {
                std::size_t start = message.find(":") + 1;
                std::size_t end = message.find("!");
                if (start != std::string::npos && end != std::string::npos) {
                    nickname = message.substr(start, end - start);
                }
                sendCommand("NOTICE " + nickname + " :\001VERSION IRCClient 1.0\001");
            }
            else if (message.find("NOTICE * :*** No Ident response") != std::string::npos) {
                if (!nickname.empty()) {
                    sendCommand("USER " + nickname + " " + nickname + " irc.rizon.net :" + nickname);
                }
            }
        }
    }

    void inputLoop() {
        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input.empty()) continue;

            sendCommand(input);

            if (input == "QUIT") {
                break;
            }
        }
    }

    std::string ip_;
    int port_;
    int sockfd_;
};

int main() {
    try {
        int option;
        std::cout << "Enter 1 to connect to the default server, or any other number to enter a custom server: ";
        std::cin >> option;
        std::cin.ignore();

        std::string ip;
        int port;

        if (option == 1) {
            ip = "52.193.79.145";
            port = 6667;
        } else {
            ip = "127.0.0.1";
            port = 6667;
        }

        IRCClient client(ip, port);
        client.connectToServer();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

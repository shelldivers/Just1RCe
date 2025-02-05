// g++ -o client_test -std=c++11 tests/test_client_for_commands_mac_realtime.cc
// ./client_test
// 입장시 NICK, USER 명령어를 알맞게 입력하면 접속이 된다
// 일반적으로 이름의 크기는 9자 제한이 있으므로 
// 추가적으로 클라이언트를 연결해 메시지를 확인하기 위해서는 8글자 + 1숫자 로 표기하는 것이 좋다
// NICK nickname   
// USER username 0 * :realname
// 명령어를 양식에 맞게 올바르게 입력하면 서버에서 알맞은 동작을 하며 서버에서 클라이언트에게 보내는
// 메시지 양식을 그대로 보여준다

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

        // 🔹 std::thread에서 멤버 함수를 실행할 수 있도록 람다 표현식 사용
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

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            int bytes_received = recv(sockfd_, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                std::cout << "Disconnected from server." << std::endl;
                break;
            }
            std::cout << "[RECEIVED]: " << buffer;

            // PING 처리
            if (std::strncmp(buffer, "PING", 4) == 0) {
                std::string pong_response = std::string(buffer).replace(0, 4, "PONG");
                std::cout << "[DEBUG] Responding to PING with: " << pong_response << std::endl;
                sendCommand(pong_response);
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
        std::string ip = "127.0.0.1";  // localhost (변경됨)
        int port = 6667;       // 기본 포트

        IRCClient client(ip, port);
        client.connectToServer();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
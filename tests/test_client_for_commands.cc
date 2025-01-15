#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

class IRCClient {
public:
    IRCClient(const std::string& ip, int port, const std::string& nickname)
        : ip_(ip), port_(port), nickname_(nickname), sockfd_(-1), ssl_ctx_(nullptr), ssl_(nullptr) {}

    ~IRCClient() {
        if (ssl_) {
            SSL_shutdown(ssl_);
            SSL_free(ssl_);
        }
        if (ssl_ctx_) {
            SSL_CTX_free(ssl_ctx_);
        }
        if (sockfd_ != -1) {
            close(sockfd_);
        }
    }

    void connectToServer() {
        // Initialize OpenSSL
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();

        ssl_ctx_ = SSL_CTX_new(TLS_client_method());
        if (!ssl_ctx_) {
            throw std::runtime_error("Failed to create SSL context");
        }

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

        ssl_ = SSL_new(ssl_ctx_);
        if (!ssl_) {
            throw std::runtime_error("Failed to create SSL object");
        }

        SSL_set_fd(ssl_, sockfd_);
        if (SSL_connect(ssl_) <= 0) {
            throw std::runtime_error("SSL/TLS handshake failed: " + getSSLError());
        }

        std::cout << "Connected securely to " << ip_ << ":" << port_ << std::endl;
        sendCommand("NICK " + nickname_);
        sendCommand("USER " + nickname_ + " 0 * :" + nickname_);

        run();
    }

private:
    void sendCommand(const std::string& command) {
        std::string formatted_command = command + "\r\n";
        if (SSL_write(ssl_, formatted_command.c_str(), formatted_command.size()) <= 0) {
            throw std::runtime_error("Failed to send command: " + command);
        }
    }

    void run() {
        char buffer[4096];

        while (true) {
            std::memset(buffer, 0, sizeof(buffer));
            int bytes_received = SSL_read(ssl_, buffer, sizeof(buffer) - 1);
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

    std::string getSSLError() {
        char error_buffer[256];
        ERR_error_string_n(ERR_get_error(), error_buffer, sizeof(error_buffer));
        return std::string(error_buffer);
    }

    std::string ip_;
    int port_;
    std::string nickname_;
    int sockfd_;
    SSL_CTX* ssl_ctx_;
    SSL* ssl_;
};

int main() {
    try {
        std::string ip = "52.193.79.145";
        int port = 6697;
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


#include "../../includes/client.h"

#include <string>

namespace Just1RCe {

Client::Client(const int listen_fd) : socket_(listen_fd) {}

Client::~Client() {}

// getter
std::string const &Client::nick_name() const { return nick_name_; }
std::string const &Client::user_name() const { return user_name_; }
std::string const &Client::real_name() const { return real_name_; }
std::string const &Client::server_name() const { return server_name_; }
std::string const &Client::away_msg() const { return away_msg_; }

// setter
void Client::set_nick_name(std::string const &name) { nick_name_ = name; }
void Client::set_user_name(std::string const &name) { user_name_ = name; }
void Client::set_real_name(std::string const &name) { real_name_ = name; }
void Client::set_server_name(std::string const &name) { server_name_ = name; }
void Client::set_away_msg(std::string const &msg) { away_msg_ = msg; }

}  // namespace Just1RCe


/*
 */
#ifndef JUST1RCE_SRCS_CLIENT_H
#define JUST1RCE_SRCS_CLIENT_H

// TODO : predefined error message

// TODO : mod flags
// describes client's permission, bitmasking
#define JUST1RCE_SRCS_CLIENT_MOD_INVISIBLE 0b00`0001
#define JUST1RCE_SRCS_CLIENT_MOD_WALLOPS 0b00`0010
#define JUST1RCE_SRCS_CLIENT_MOD_OPERATOR 0b00`0100
#define JUST1RCE_SRCS_CLIENT_MOD_REGISTERED 0b00`1000
#define JUST1RCE_SRCS_CLIENT_MOD_RECEIVE_SERVER 0B01`0000

#include <string>

#include "tcp_socket.h"

namespace Just1RCe {

class Client {
 private:
  Client();
  Client(Client const &);
  Client &operator=(Client const &);

  // newwork connection info
  // TODO : connection time control will be needed
  // ex) ping/pong
  TcpSocket socket_;

  // Identification info
  std::string nick_name_;
  std::string user_name_;
  std::string real_name_;
  std::string server_name_;

  // status info
  uint mode_mask_;
  std::string away_msg_;

  // per socket IO buffer, saving unsufficient message
  // no getter or setter
  std::string left_buffer_;

 public:
  Client(const int listen_fd);
  ~Client();

  // getter
  std::string const &nick_name() const;
  std::string const &user_name() const;
  std::string const &real_name() const;
  std::string const &server_name() const;
  std::string const &away_msg() const;

  // setter
  void set_nick_name(std::string const &name);
  void set_user_name(std::string const &name);
  void set_real_name(std::string const &name);
  void set_server_name(std::string const &name);
  void set_away_msg(std::string const &msg);

  // newwork info
  std::string GetHostName() const;
  std::string GetPortNum() const;

  // mode control
  std::string GetMode() const;
  void SetMode(uint const flag, bool const do_flag_add);
  bool CheckMode(uint const flags) const;

  // TODO : receive message, read data from socket
  // TODO : send message, write data to the socket
};

}  // namespace Just1RCe

#endif

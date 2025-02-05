
#ifndef JUST1RCE_SRCS_CLIENT_H
#define JUST1RCE_SRCS_CLIENT_H

// Predefined error message
#define JUST1RCE_SRCS_CLIENT_RECV_ERROR "Client : recv failed."
#define JUST1RCE_SRCS_CLIENT_SEND_ERROR "Client : send failed."

#define JUST1RCE_SRCS_CLIENT_MSG_MAX 512UL
#define JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM "\r\n"

#include <string>
#include <vector>

#include "../includes/ft_optional.h"
#include "../includes/tcp_socket.h"

namespace Just1RCe {

/**
 *
 * @brief Class that manage resources related to the client
 * @details This class holds newwork/identification/status information
 * @author eldeshue
 * @date 2024-12-19
 */
class Client {
 private:
  Client();
  Client(Client const &);
  Client &operator=(Client const &);

  // newwork connection info
  // TODO(eldeshue) : connection time control will be needed
  // ex) ping/pong
  TcpSocket socket_;

  // Identification info
  std::string nick_name_;
  std::string user_name_;
  std::string real_name_;
  std::string server_name_;

  // status info
  std::string away_msg_;

  // per socket IO buffer, saving unsufficient message
  // no getter or setter
  std::string read_buffer_;
  std::string write_buffer_;

 public:
  explicit Client(const int listen_fd);
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

  // network info
  std::string GetHostName() const;
  std::string GetPortNum() const;

  // buffered io
  ft::optional<std::vector<std::string> > GetReceivedMessages();

  void SetSendMessage(std::string const &message);
  bool SendMessage();
};

}  // namespace Just1RCe

#endif

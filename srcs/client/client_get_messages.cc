
extern "C" {
#include <sys/socket.h>
}

#include <cstring>
#include <stdexcept>

#include "../../includes/client.h"

namespace Just1RCe {

/**
 * @brief receive raw text from socket and turn it to propeer messages
 * @return vector of strings
 * @throws runtime error, recv failure
 */
std::vector<std::string> Client::GetReceivedMessages() {
  char recv_buffer[JUST1RCE_SRCS_CLIENT_MSG_MAX + 1];

  memset(recv_buffer, '\0', JUST1RCE_SRCS_CLIENT_MSG_MAX + 1);
  int recv_size =
      recv(socket_.socket_fd(), recv_buffer, JUST1RCE_SRCS_CLIENT_MSG_MAX, 0);
  if (recv_size == -1) {
    throw std::runtime_error(JUST1RCE_SRCS_CLIENT_RECV_ERROR);
  }

  std::vector<std::string> result;
  char *start_pos = recv_buffer, *end_pos = NULL;
  if (recv_size > 0) {
    // search CR-LF to identify whole message
    while (end_pos =
               std::strstr(start_pos, JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM)) {
      // get whole message from buffer
      const int msg_size = end_pos - start_pos;
      std::string msg(start_pos, msg_size);

      // combine with the previous left buffer
      if (!read_buffer_.empty()) {
        msg = read_buffer_ + msg;
        read_buffer_.clear();
      }
      // push whole message
      result.push_back(msg);

      // move search position
      recv_size -= msg_size + 2;  // size left in the buffer
      start_pos = end_pos + 2;    // move after cr-lf
    }
  }
  // save left over text
  read_buffer_ = std::string(start_pos, recv_size);

  // return
  return result;
}

}  // namespace Just1RCe

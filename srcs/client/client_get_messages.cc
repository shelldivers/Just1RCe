
extern "C" {
#include <errno.h>
#include <sys/socket.h>
}

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../includes/client.h"

namespace Just1RCe {

/**
 * @brief receive raw text from socket and turn it to propeer messages
 * @return vector of strings
 * @throws runtime error, recv failure
 */
ft::optional<std::vector<std::string> > Client::GetReceivedMessages() {
  char recv_buffer[JUST1RCE_SRCS_CLIENT_MSG_MAX + 1];

  memset(recv_buffer, '\0', JUST1RCE_SRCS_CLIENT_MSG_MAX + 1);
  ssize_t recv_size =
      recv(socket_.socket_fd(), recv_buffer, JUST1RCE_SRCS_CLIENT_MSG_MAX, 0);
  if (recv_size == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return ft::optional<std::vector<std::string> >();  // empty option, equivalent to std::noops
    }
    throw std::runtime_error(JUST1RCE_SRCS_CLIENT_RECV_ERROR);
  }

  // get result
  ft::optional<std::vector<std::string> > result;
  size_t start_pos = 0, end_pos = 0;
  if (recv_size > 0) {
    // combine recv_buffer and left over read_buffer_
    read_buffer_.append(recv_buffer, recv_size);

    // split result with delimeter
    while ((end_pos = read_buffer_.find(JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM,
                                        start_pos)) != std::string::npos) {
      // result is option, use after initialization
      if (!result.has_value()) {
        *result = std::vector<std::string>();
      }

      // get whole message from read_buffer
      std::string msg = read_buffer_.substr(start_pos, (end_pos - start_pos));

      // push whole message
      (*result).push_back(msg);

      // move search position
      start_pos = end_pos + 2;  // move after cr-lf
    }
  }
  // save left over text
  // erase used messages
  if (start_pos != 0)
    read_buffer_ =
        read_buffer_.substr(start_pos, read_buffer_.size() - start_pos);

  return result;
}

}  // namespace Just1RCe

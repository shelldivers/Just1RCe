

extern "C" {
#include <errno.h>
#include <sys/socket.h>
}

#include <stdexcept>

#include "../../includes/client.h"

namespace Just1RCe {

/**
 * @brief
 * @param
 */
void Client::setSendMessage(std::string const &message) {
  write_buffer_ += message;
}

/**
 * @brief
 * @details
 * @return
 */
bool Client::sendMessage() {
  // send call
  int send_size =
      send(socket_.socket_fd(), write_buffer_.c_str(),
           std::min(write_buffer_.size(), JUST1RCE_SRCS_CLIENT_MSG_MAX), 0);
  if (send_size == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return true;  // re-register the write event
    } else {
      // throw error
      throw std::runtime_error(JUST1RCE_SRCS_CLIENT_SEND_ERROR);
    }
  }

  // save substr

  // return is buffer left
}

}  // namespace Just1RCe

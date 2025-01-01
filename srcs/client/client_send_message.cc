

extern "C" {
#include <errno.h>
#include <sys/socket.h>
}

#include <algorithm>
#include <stdexcept>
#include <string>

#include "../../includes/client.h"

namespace Just1RCe {

/**
 * @brief add processed message data to the write buffer
 * @attention cr-lf will be added here
 * @param message pure message data without delimeter(cr-lf)
 */
void Client::SetSendMessage(std::string const &message) {
  write_buffer_ += message + JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM;
}

/**
 * @brief send text data to the client
 * @details will be called after write event triggered
 * @attention write event will be level triggered
 * @return if true, write_buffer_ has content to write, register write event
 */
bool Client::SendMessage() {
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

  // save left buffer
  if (send_size > 0) write_buffer_ = write_buffer_.substr(send_size);

  // do re-register write event?
  return !write_buffer_.empty();
}

}  // namespace Just1RCe

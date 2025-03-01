

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
 * @attention register write event with EPOLLET | EPOLLONESHOT
 * @param message pure message data without delimeter(cr-lf)
 */
void Client::SetSendMessage(std::string const &message) {
  write_buffer_ += message + JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM;
}

/**
 * @brief send text data to the client
 * @details will be called after write event triggered
 * @attention write event will be marked as EPOLLET | EPOLLONESHOT
 * @return if true, write_buffer_ has content to send, re-register write event
 */
bool Client::SendMessage() {
  ssize_t send_size = send(socket_.socket_fd(), write_buffer_.c_str(),
                           write_buffer_.size(), MSG_NOSIGNAL);
  if (send_size == -1) {
      // throw error
      throw std::runtime_error(JUST1RCE_SRCS_CLIENT_SEND_ERROR);
  }

  // save partial message, push back to the read_buffer_
  // send_size == 0 means no change, no need to call substr
  // substr takess first position of original string to copy
  if (send_size > 0) write_buffer_ = write_buffer_.substr(send_size);

  // if true, there is content to send, re-register event
  return !write_buffer_.empty();
}

}  // namespace Just1RCe

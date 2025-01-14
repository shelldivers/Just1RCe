
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
 * @brief recv text from socket's fd and append text to the buffer
 * @details append pbuffer with the text from fd's socket till the system buffer
 * is empty.
 * @throw Throw runtime exception if the recv calls failed.
 * @param fd fd of the socket to recv
 * @param pbuffer pointer to the buffer to save received text
 * @return if the recv succeed, return true.
 */
static bool recv_and_append_buffer(int const fd, std::string *pbuffer) {
  char recv_buffer[JUST1RCE_SRCS_CLIENT_MSG_MAX + 1];
  ssize_t recv_size = 0;

  memset(recv_buffer, '\0', JUST1RCE_SRCS_CLIENT_MSG_MAX + 1);
  while (recv_size = recv(fd, recv_buffer, JUST1RCE_SRCS_CLIENT_MSG_MAX, 0)) {
    // recv failed
    if (recv_size == -1) {
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return false;  // empty option,
                       // equivalent to
                       // std::noops
      }
      throw std::runtime_error(JUST1RCE_SRCS_CLIENT_RECV_ERROR);
    }
    // recv succed, append buffer with the received text
    pbuffer->append(recv_buffer, recv_size);
  }
  return true;
}

/**
 * @brief split string with CR-LF
 * @details Split string with "\r\n" and save them to the vector of string.
 * Splitted strings does not contain "\r\n".
 * @param pstr pointer to the string to split
 * @return vector of splitted string
 */
static std::vector<std::string> split_string_with_crlf(std::string *pstr) {
  std::vector<std::string> buffered_messages;
  size_t start_pos = 0, end_pos = 0;

  while ((end_pos = pstr->find(JUST1RCE_SRCS_CLIENT_MESSAGE_DELIM,
                               start_pos)) != std::string::npos) {
    // get whole message from str
    buffered_messages.push_back(pstr->substr(start_pos, (end_pos - start_pos)));

    // move start position to search next delimeter delimeter
    start_pos = end_pos + 2;  // move after cr-lf
  }
  // save left over text
  // erase used messages
  if (start_pos != 0) *pstr = pstr->substr(start_pos, pstr->size() - start_pos);
  return buffered_messages;
}

/**
 * @brief receive all the text from socket and turn it to propeer messages
 * @return optional vector of messages(strings)
 * @throws runtime error, recv failure
 */
ft::optional<std::vector<std::string> > Client::GetReceivedMessages() {
  if (!recv_and_append_buffer(this->socket_.socket_fd(), &(this->read_buffer_)))
    return ft::optional<std::vector<std::string> >();

  return ft::optional<std::vector<std::string> >(
      split_string_with_crlf(&(this->read_buffer_)));
}

}  // namespace Just1RCe


#include <sstream>
#include <string>

#include "../../includes/client.h"

namespace Just1RCe {

/**
 * @brief get IP address in IPv4 formatted string
 * @return formatted string
 * @throws none
 */
std::string Client::GetHostName() const {
  static uint const octet_mask = 0b11111111;  // mask of octet
  uint const ip_address = socket_.GetIPAddress();
  std::ostringstream oss;

  for (int offset = 3; offset >= 0; --offset) {
    // extracting field's octet : use bit-masking
    uint const field_octet = ip_address & (octet_mask << (offset * 8));

    // getting field : shift extracted octet to erase trailing zero bits
    uint const field = field_octet >> ((3 - offset) * 8);

    // append result
    oss << field;
    if (offset != 0) oss << '.';  // skip last dot
  }
  return oss.str();
}

/**
 * @brief get port number in string
 * @return string
 * @throws none
 */
std::string Client::GetPortNum() const {
  std::ostringstream oss;
  oss << socket_.GetPortNum();
  return oss.str();
}

int Client::GetFd() const { return socket_.socket_fd(); }

}  // namespace Just1RCe

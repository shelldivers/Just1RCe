
#include <sstream>

#include "client.h"

namespace Just1RCe {

/**
 * @brief get IP address in IPv4 formatted string
 * @return formatted string
 */
std::string Client::GetHostName() const {
  static uint const mask = 0b11111111000000000000000000000000;
  uint const ip_address = socket_.GetIPAddress();
  std::ostringstream oss;

  for (int i = 0; i < 4; ++i) {
    uint const field = (ip_address & (mask >> (i * 8))) >> ((4 - i) * 8);
    oss << field;
    if (i != 3) oss << '.';
  }
  return oss.str();
}

/**
 * @brief get port number in string
 * @return string
 */
std::string Client::GetPortNum() const {
  std::ostringstream oss;
  oss << socket_.GetPortNum();
  return oss.str();
}

}  // namespace Just1RCe

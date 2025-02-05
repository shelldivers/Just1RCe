#include <string>
#include <vector>

#include "../../includes/parse.h"

namespace Just1RCe {

std::vector<std::string> CreateTokenStream(const std::string &message) {
  std::vector<std::string> token_stream;
  int current_index = 0;
  int before_index;

  if (message.empty()) return token_stream;

  while (message[current_index++] != JUST1RCE_SPACE);
  token_stream.push_back(message.substr(0, current_index - 1));
  before_index = current_index - 1;

  while (current_index++ < message.size() &&
         token_stream.size() < JUST1RCE_MAX_MIDDLE_SIZE) {
    if (message[current_index] == JUST1RCE_SPACE) {
      token_stream.push_back(
          message.substr(before_index + 1, current_index - before_index - 1));
      before_index = current_index;
    } else if (message[current_index] == JUST1RCE_COLON) {
      token_stream.push_back(message.substr(current_index + 1));
      return token_stream;
    }
  }

  if (token_stream.size() == JUST1RCE_MAX_MIDDLE_SIZE) {
    size_t colon_pos = message.find(':');
    if (colon_pos != std::string::npos)
      token_stream.push_back(message.substr(colon_pos + 1));
  } else
    token_stream.push_back(
        message.substr(before_index + 1, current_index - before_index));

  return token_stream;
}

}  // namespace Just1RCe

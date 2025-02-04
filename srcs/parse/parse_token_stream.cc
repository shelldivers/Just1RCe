#include <string>
#include <vector>

#include "../../includes/parse.h"

namespace Just1RCe {

void NickParser(std::vector<std::string> &token_stream, std::string &nickname) {
  nickname = token_stream[1];
  return true;
}

void UserParser(std::vector<std::string> &token_stream, std::string &username,
                std::string &realname) {
  username = token_stream[1];
  realname = token_stream[4];

  return true;
}

void PingParser(std::vector<std::string> &token_stream, std::string &token) {
  token = token_stream[1];

  return true;
}

void PongParser(std::vector<std::string> &token_stream, std::string &token) {
  token = token_stream[1];
}

void QuitParser(std::vector<std::string> &token_stream, std::string &reason) {
  reason = token_stream[1];
}

void JoinParser(std::vector<std::string> &token_stream,
                std::vector<std::string> &channels,
                std::vector<std::string> &passwords)
};  // namespace Just1RCe
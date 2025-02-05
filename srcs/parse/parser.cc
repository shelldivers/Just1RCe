#include <string>
#include <vector>

#include "../../includes/parse.h"
#include "../../includes/numeric.h"

namespace Just1RCe {

/**
 * @brief NickParser function checks 2
 * 1. Is nickname ha 
 * 
 * 
*/
const int NickParser(std::vector<std::string> &token_stream, std::string &nickname) {
  if (token_stream.size() < 2 || token_stream[1].empty()) return ERR_NONICKNAMEGIVEN;

  const std::string not_contain = {" ,*?!@."};
  const std::string not_start = {"$:#&"};
  
  nickname = token_stream[1];
  for (int idx = 0; idx < not_start.size(); ++idx) {
    if (nickname[0] == not_start[idx]) return ERR_ERRONEUSNICKNAME;
  }

  for (int nickname_idx = 0; nickname_idx < nickname.size(); ++nickname_idx) {
    for (int not_contain_idx = 0; not_contain_idx < not_contain.size(); ++ not_contain_idx) {
      if (nickname[nickname_idx] == not_contain[not_contain_idx]) return ERR_ERRONEUSNICKNAME;
    }
  }
}

const int UserParser(std::vector<std::string> &token_stream, std::string &username,
                std::string &realname) {
  username = token_stream[1];
  realname = token_stream[4];


}

const int PingParser(std::vector<std::string> &token_stream, std::string &token) {
  token = token_stream[1];


}

const int PongParser(std::vector<std::string> &token_stream, std::string &token) {
  token = token_stream[1];
}

const int QuitParser(std::vector<std::string> &token_stream, std::string &reason) {
  reason = token_stream[1];
}

const int JoinParser(std::vector<std::string> &token_stream,
                std::vector<std::string> &channels,
                std::vector<std::string> &passwords)
}  // namespace Just1RCe
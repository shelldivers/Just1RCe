/**
 * @file parser.cc
 * @brief Contains parse functions for all commands.
 * 
 * @todo class로 만들어서 객체로 만들기
 * 
 * These parse functions operate under the assumption that the input arguments are empty strings.
 * The functions extract and store relevant information from command tokens.
 * If not a valid input exists do nothing.
 * 
 * @author KenWR
 * @date 2025-02-05
 */

#include <string>
#include <vector>

#include "../../includes/parse.h"
#include "../../includes/numeric.h"

namespace Just1RCe {

/**
 * @brief Parses the NICK command.
 * 
 * Extracts a nickname from the token stream if a valid input exists.
 * 
 * @param token_stream A vector of strings representing the command arguments.
 * @param nickname A reference to the string where the parsed nickname is stored.
 */
void NickParser(std::vector<std::string> &token_stream, std::string &nickname) {
  if (token_stream.size() < 2) return;

  nickname = token_stream[1];
}

/**
 * @brief Parses the USER command.
 * 
 * Extracts a username, realname from the token stream if a valid input exists.
 * 
 * @param token_stream A vector of strings representing the command arguments.
 * @param nickname A reference to the string where the parsed nickname is stored.
 */
void UserParser(std::vector<std::string> &token_stream, std::string &username,
                std::string &realname) {
  if (token_stream.size() < 5) return;

  username = token_stream[1];
  realname = token_stream[4];
}


void PingParser(std::vector<std::string> &token_stream, std::string &token) {
  if (token_stream.size() < 2) return;

  token = token_stream[1];
}


void PongParser(std::vector<std::string> &token_stream, std::string &token) {
  if (token_stream.size() < 5)
  token = token_stream[1];
}

void QuitParser(std::vector<std::string> &token_stream, std::string &reason) {
  reason = token_stream[1];
}

void JoinParser(std::vector<std::string> &token_stream,
                std::vector<std::string> &channels,
                std::vector<std::string> &passwords) {
  

}

}  // namespace Just1RCe
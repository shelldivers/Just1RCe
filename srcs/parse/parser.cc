#include "../../includes/parser.h"

#include <string>
#include <vector>

#include "../../includes/numeric.h"

namespace Just1RCe {

Parser::Parser(const std::string &message) {
  splitBySpace(message);
}

Parser::~Parser() {}

void Parser::splitBySpace(const std::string &message) {
  size_t start = 0, end;

  if (message.empty()) return;

  while ((end = message.find(JUST1RCE_SPACE, start)) != std::string::npos) {
    std::string splited = message.substr(start, end - start);
    if (splited.size())
      token_stream_.push_back(splited);
    start = end + 1;
  }
  std::string splited = message.substr(start);
  if (splited.size())
    token_stream_.push_back(splited);
}

std::vector<std::string> Parser::splitByComma(const std::string &param) {
  std::vector<std::string> tokens;
  size_t start = 0, end;

  if (param.empty()) return tokens;

  while ((end = param.find(JUST1RCE_COMMA, start)) != std::string::npos) {
    std::string splited = param.substr(start, end - start);
    if (splited.size())
      tokens.push_back(splited);
    start = end + 1;
  }
  std::string splited = param.substr(start);
  if (splited.size())
    token_stream_.push_back(splited);

  return tokens;
}

const int Parser::ParseCommandNick(std::string *nickname) {
  if (token_stream_.size() < 2) return ERR_NONICKNAMEGIVEN;

  *nickname = token_stream_[1];

  return CMD_SUCCESS;
}

const int Parser::ParseCommandUser(std::string *username,
                                   std::string *realname) {
  if (token_stream_.size() < 5) return ERR_NEEDMOREPARAMS;

  *username = token_stream_[1];
  *realname = token_stream_[4];

  return CMD_SUCCESS;
}

const int Parser::ParseCommandPing(std::string *token) {
  if (token_stream_.size() < 2) return ERR_NOORIGIN;

  *token = token_stream_[1];

  return CMD_SUCCESS;
}

const int Parser::ParseCommandQuit(std::string *reason) {
  if (token_stream_.size() < 2)
    *reason = DEFAULT_QUIT_COMMENT;
  else
    *reason = token_stream_[1];

  return CMD_SUCCESS;
}

const int Parser::ParseCommandJoin(std::vector<std::string> *channels,
                                   std::vector<std::string> *passwords) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  *channels = splitByComma(token_stream_[1]);
  if (token_stream_.size() > 2) *passwords = splitByComma(token_stream_[2]);

  return CMD_SUCCESS;
}

const int Parser::ParseCommandPart(std::vector<std::string> *channels) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  *channels = splitByComma(token_stream_[1]);

  return CMD_SUCCESS;
}

const int Parser::ParseCommandTopic(std::string *channel, std::string *topic) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  *channel = token_stream_[1];
  if (token_stream_.size() > 2)
    *topic = token_stream_[2];
  else
    *topic = "";

  return CMD_SUCCESS;
}

const int Parser::ParseCommandNames(std::string *channel) {
  if (token_stream_.size() < 2) *channel = "";

  return CMD_SUCCESS;
}

const int Parser::ParseCommandList(std::string *channel) {
  if (token_stream_.size() < 2) *channel = "";

  return CMD_SUCCESS;
}

const int Parser::ParseCommandInvite(std::string *nickname,
                                     std::string *channel) {
  if (token_stream_.size() < 3) return ERR_NEEDMOREPARAMS;

  *nickname = token_stream_[1];
  *channel = token_stream_[2];

  return CMD_SUCCESS;
}

const int Parser::ParseCommandKick(std::string *channel, std::string *nickname,
                                   std::string *comment) {
  if (token_stream_.size() < 3) return ERR_NEEDMOREPARAMS;

  *channel = token_stream_[1];
  *nickname = token_stream_[2];
  if (token_stream_.size() > 3)
    *comment = token_stream_[3];
  else
    *comment = DEFAULT_KICK_COMMENT;

  return CMD_SUCCESS;
}

const int Parser::ParseCommandMode(std::string *target,
                                   std::string *mode_string,
                                   std::string *mode_argument) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  *target = token_stream_[1];
  if (token_stream_.size() > 2)
    *mode_string = token_stream_[2];
  else
    *mode_string = "";
  if (token_stream_.size() > 3)
    *mode_argument = token_stream_[3];
  else
    *mode_argument = "";

  return CMD_SUCCESS;
}

const int Parser::ParseCommandPrivmsg(std::string *target,
                                      std::string *text_to_send) {
  if (token_stream_.size() < 2) return ERR_NORECIPIENT;
  if (token_stream_.size() < 3) return ERR_NOTEXTTOSEND;

  *target = token_stream_[1];
  *text_to_send = token_stream_[2];

  return CMD_SUCCESS;
}

}  // namespace Just1RCe

#include "../../includes/tokenizer.h"

#include <string>
#include <vector>

#include "../../includes/numeric.h"

namespace Just1RCe {

Tokenizer::Tokenizer(const std::string &message)
    : token_stream_(splitBySpace(message)) {}

Tokenizer::~Tokenizer() {}

std::vector<std::string> Tokenizer::splitBySpace(const std::string &message) {
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

  if (token_stream_.size() == JUST1RCE_MAX_MIDDLE_SIZE) {
    size_t colon_pos = message.find(':');
    if (colon_pos != std::string::npos)
      token_stream_.push_back(message.substr(colon_pos + 1));
  } else
    token_stream_.push_back(
        message.substr(before_index + 1, current_index - before_index));

  return token_stream_;
}

std::vector<std::string> Tokenizer::splitByComma(const std::string &param) {
  std::vector<std::string> tokens;
  const char delimeter = ',';
  size_t start = 0, end;

  while ((end = param.find(delimeter, start)) != std::string::npos) {
    tokens.push_back(param.substr(start, end - start));
    start = end + 1;
  }
  tokens.push_back(param.substr(start));

  return tokens;
}

const int Tokenizer::NickTokenize(std::string &nickname) {
  if (token_stream_.size() < 2) return ERR_NONICKNAMEGIVEN;

  nickname = token_stream_[1];

  return CMD_SUCCESS;
}

const int Tokenizer::UserTokenize(std::string &username,
                                  std::string &realname) {
  if (token_stream_.size() < 5) return ERR_NEEDMOREPARAMS;

  username = token_stream_[1];
  realname = token_stream_[4];

  return CMD_SUCCESS;
}

const int Tokenizer::PingTokenize(std::string &token) {
  if (token_stream_.size() < 2) return ERR_NOORIGIN;

  token = token_stream_[1];

  return CMD_SUCCESS;
}

const int Tokenizer::QuitTokenize(std::string &reason) {
  if (token_stream_.size() < 2)
    reason = DEFAULT_QUIT_COMMENT;
  else
    reason = token_stream_[1];

  return CMD_SUCCESS;
}

const int Tokenizer::JoinTokenize(std::vector<std::string> &channels,
                                  std::vector<std::string> &passwords) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  channels = splitByComma(token_stream_[1]);
  if (token_stream_.size() > 2) passwords = splitByComma(token_stream_[2]);

  return CMD_SUCCESS;
}

const int Tokenizer::PartTokenize(std::vector<std::string> &channels) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  channels = splitByComma(token_stream_[1]);

  return CMD_SUCCESS;
}

const int Tokenizer::TopicTokenize(std::string &channel, std::string &topic) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  channel = token_stream_[1];
  if (token_stream_.size() > 2) topic = token_stream_[2];
}

const int Tokenizer::NamesTokenize(std::string &channel) {
  if (token_stream_.size() < 2) channel = "";

  return CMD_SUCCESS;
}

const int Tokenizer::ListTokenize(std::string &channel) {
  if (token_stream_.size() < 2) channel = "";

  return CMD_SUCCESS;
}

const int Tokenizer::InviteTokenize(std::string &nickname,
                                    std::string &channel) {
  if (token_stream_.size() < 3) return ERR_NEEDMOREPARAMS;

  nickname = token_stream_[1];
  channel = token_stream_[2];

  return CMD_SUCCESS;
}

const int Tokenizer::KickTokenize(std::string &channel, std::string &nickname,
                                  std::string &comment) {
  if (token_stream_.size() < 3) return ERR_NEEDMOREPARAMS;

  channel = token_stream_[1];
  nickname = token_stream_[2];
  if (token_stream_.size() > 3)
    comment = DEFAULT_KICK_COMMENT;
  else
    comment = token_stream_[3];

  return CMD_SUCCESS;
}

const int Tokenizer::ModeTokenize(std::string &target, std::string &mode_string,
                                  std::string &mode_argument) {
  if (token_stream_.size() < 2) return ERR_NEEDMOREPARAMS;

  target = token_stream_[1];
  if (token_stream_.size() > 2)
    mode_string = token_stream_[2];
  else
    mode_string = "";
  if (token_stream_.size() > 3)
    mode_argument = token_stream_[3];
  else
    mode_argument = "";

  return CMD_SUCCESS;
}

const int Tokenizer::PrivmsgTokenize(std::string &target,
                                     std::string &text_to_send) {
  if (token_stream_.size() < 2) return ERR_NORECIPIENT;
  if (token_stream_.size() < 3) return ERR_NOTEXTTOSEND;

  target = token_stream_[1];
  text_to_send = token_stream_[2];

  return CMD_SUCCESS;
}

}  // namespace Just1RCe

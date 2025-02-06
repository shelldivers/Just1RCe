#ifndef JUST1RCE_INCLUDES_LEXER_H
#define JUST1RCE_INCLUDES_LEXER_H

#include <string>
#include <vector>

#define JUST1RCE_MAX_MIDDLE_SIZE 14
#define JUST1RCE_SPACE ' '
#define JUST1RCE_COLON ':'
#define DEFAULT_QUIT_COMMENT "Leaving"
#define DEFAULT_KICK_COMMENT "For some reason"

namespace Just1RCe {

class Tokenizer {
 private:
  std::vector<std::string> token_stream_;
  std::vector<std::string> splitBySpace(const std::string &message);
  std::vector<std::string> splitByComma(const std::string &param);
  Tokenizer(const Tokenizer &rhs);
  Tokenizer &operator=(const Tokenizer &rhs);

 public:
  Tokenizer(const std::string &message);
  ~Tokenizer();
  const int NickTokenize(std::string &nickname);
  const int UserTokenize(std::string &username, std::string &realname);
  const int PingTokenize(std::string &token);
  const int QuitTokenize(std::string &reason);
  const int JoinTokenize(std::vector<std::string> &channels,
                         std::vector<std::string> &passwords);
  const int PartTokenize(std::vector<std::string> &channels);
  const int TopicTokenize(std::string &channel, std::string &topic);
  const int NamesTokenize(std::string &channel);
  const int ListTokenize(std::string &channel);
  const int InviteTokenize(std::string &nickname, std::string &channel);
  const int KickTokenize(std::string &channel, std::string &nick_name,
                         std::string &comment);
  const int ModeTokenize(std::string &target, std::string &mode_string,
                         std::string &mode_argument);
  const int PrivmsgTokenize(std::string &target, std::string &text_to_send);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_PARSE_H
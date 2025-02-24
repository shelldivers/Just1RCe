#ifndef JUST1RCE_INCLUDES_PARSER_H
#define JUST1RCE_INCLUDES_PARSER_H

#include <string>
#include <vector>

#define JUST1RCE_MAX_MIDDLE_SIZE 14
#define JUST1RCE_SPACE ' '
#define JUST1RCE_COLON ':'
#define JUST1RCE_COMMA ','
#define JUST1RCE_DEFAULT_QUIT_COMMENT "Leaving"
#define JUST1RCE_DEFAULT_KICK_COMMENT "For some reason"

namespace Just1RCe {

class Parser {
 private:
  std::vector<std::string> token_stream_;
  void splitBySpace(const std::string &message);
  std::vector<std::string> splitByComma(const std::string &param);
  Parser(const Parser &);
  Parser &operator=(const Parser &);

 public:
  explicit Parser(const std::string &message);
  ~Parser();

  std::vector<std::string> GetTokenStream() const;

  int ParseCommandPass(std::string *password);
  int ParseCommandNick(std::string *nickname);
  int ParseCommandUser(std::string *username, std::string *realname);
  int ParseCommandPing(std::string *token);
  int ParseCommandQuit(std::string *reason);
  int ParseCommandJoin(std::vector<std::string> *channels,
                       std::vector<std::string> *passwords);
  int ParseCommandPart(std::vector<std::string> *channels);
  int ParseCommandTopic(std::string *channel, std::string *topic);
  int ParseCommandNames(std::string *channel);
  int ParseCommandList(std::string *channel);
  int ParseCommandInvite(std::string *nickname, std::string *channel);
  int ParseCommandKick(std::string *channel, std::string *nick_name,
                       std::string *comment);
  int ParseCommandMode(std::string *target, std::string *mode_string,
                       std::string *mode_argument);
  int ParseCommandPrivmsg(std::string *target, std::string *text_to_send);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_PARSE_H

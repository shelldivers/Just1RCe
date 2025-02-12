#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H

#include <string.h>

namespace Just1RCe {

#include "../command_handler.h";
class ContextHolder;

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);
  void isNicknameUse(const std::string& nickname);
  void isNicknameErroneus(const std::string& nickname);
  void is
  std::string combineStringToMessage(std::string source, std::string Numeric,);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  std::vector<int> operator()(const int fd, const std::string& message);
};

}  // namespace Just1RCe

#endif
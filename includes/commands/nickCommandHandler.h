#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H

#include <string>

namespace Just1RCe {

#include "../command_handler.h";
class ContextHolder;

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  void operator()(const int fd, const std::string& meesage);
};

} // namespace Just1RCe

#endif
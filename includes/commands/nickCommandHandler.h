#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H

#include <string.h>

#include <vector>

#include "../command_handler.h"
class DbContext;

namespace Just1RCe {

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif
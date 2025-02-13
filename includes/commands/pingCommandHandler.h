#ifndef JUST1RCE_INCLUDES_COMMANDS_PING_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_PING_COMMAND_HANDLER_H

#include <string.h>

#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class PingCommandHandler : public CommandHandler {
 private:
  PingCommandHandler(PingCommandHandler const &);
  PingCommandHandler &operator=(PingCommandHandler const &);

 public:
  PingCommandHandler();
  ~PingCommandHandler();

  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif

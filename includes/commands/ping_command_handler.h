#ifndef JUST1RCE_COMMANDS_PING_COMMAND_HANDLER_H_
#define JUST1RCE_COMMANDS_PING_COMMAND_HANDLER_H_

#include <string.h>

#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Client;

class PingCommandHandler : public CommandHandler {
 private:
  PingCommandHandler(PingCommandHandler const &);
  PingCommandHandler &operator=(PingCommandHandler const &);
  const int CheckPing(const std::string &token);

 public:
  PingCommandHandler();
  ~PingCommandHandler();

  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif

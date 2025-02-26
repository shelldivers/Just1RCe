#ifndef JUST1RCE_INCLUDES_COMMANDS_KICK_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_KICK_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Client;

class KickCommandHandler : public CommandHandler {
 private:
  KickCommandHandler(const KickCommandHandler &);
  KickCommandHandler &operator=(const KickCommandHandler &);

 public:
  KickCommandHandler();
  ~KickCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_Kick_COMMANDHANDLER_H_
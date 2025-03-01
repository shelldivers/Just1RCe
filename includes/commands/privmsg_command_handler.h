#ifndef JUST1RCE_INCLUDES_COMMANDS_Privmsg_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_Privmsg_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Client;

class PrivmsgCommandHandler : public CommandHandler {
 private:
  PrivmsgCommandHandler(const PrivmsgCommandHandler &);
  PrivmsgCommandHandler &operator=(const PrivmsgCommandHandler &);

 public:
  PrivmsgCommandHandler();
  ~PrivmsgCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_Privmsg_COMMANDHANDLER_H_
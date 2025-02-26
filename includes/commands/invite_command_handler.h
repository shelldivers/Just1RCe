#ifndef JUST1RCE_INCLUDES_COMMANDS_INVITE_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_INVITE_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Client;

class InviteCommandHandler : public CommandHandler {
 private:
  InviteCommandHandler(const InviteCommandHandler &);
  InviteCommandHandler &operator=(const InviteCommandHandler &);

 public:
  InviteCommandHandler();
  ~InviteCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_Invite_COMMANDHANDLER_H_
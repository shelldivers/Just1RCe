#ifndef JUST1RCE_INCLUDES_COMMANDS_USER_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_USER_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Client;

class UserCommandHandler : public CommandHandler {
 private:
  UserCommandHandler(const UserCommandHandler &);
  UserCommandHandler &operator=(const UserCommandHandler &);
  int CheckUser(const Client &client, std::string username,
                std::string realname);

 public:
  UserCommandHandler();
  ~UserCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_USERCOMMANDHANDLER_H_

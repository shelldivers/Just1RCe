#ifndef JUST1RCE_INCLUDES_COMMANDS_PASS_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_PASS_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

class Client;

namespace Just1RCe {

class PassCommandHandler : public CommandHandler {
 private:
  PassCommandHandler(const PassCommandHandler &);
  PassCommandHandler &operator=(const PassCommandHandler &);
  const int GetUserErrorCode(const Client &client, std::string password);

 public:
  PassCommandHandler();
  ~PassCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_PASSCOMMANDHANDLER_H_


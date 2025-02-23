#ifndef JUST1RCE_INCLUDES_COMMANDS_PASS_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_PASS_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

class Client;

namespace Just1RCe {

class PassCommandHandler : public CommandHandler {
 private:
  std::string password_;

  PassCommandHandler(const PassCommandHandler &);
  PassCommandHandler &operator=(const PassCommandHandler &);
  int CheckPass(const Client &client, std::string password);

 public:
  explicit PassCommandHandler(const std::string &password);
  ~PassCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_PASSCOMMANDHANDLER_H_

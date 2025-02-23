#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"
class DbContext;
class Client;

namespace Just1RCe {

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);
  int CheckNick(const Client& client, const std::string& new_nickname);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_

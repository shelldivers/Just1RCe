#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class DbContext;
class Client;

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);
  int CheckNick(const std::string& new_nickname);
  void AnnounceNickChanged(Client* client,
                           const std::string& old_nickname,
                           const std::string& new_nickname,
                           std::vector<int>* fd_list);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H_

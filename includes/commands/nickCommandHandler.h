#ifndef JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_NICK_COMMAND_HANDLER_H

#include <string.h>

namespace Just1RCe {

#include "../command_handler.h";
class ContextHolder;

class NickCommandHandler : public Just1RCe::CommandHandler {
 private:
  NickCommandHandler(const NickCommandHandler& rhs);
  NickCommandHandler& operator=(const NickCommandHandler& rhs);
  const int NickCommandHandler::checkNicknameForbidden(
      const std::string& nickname);
  const int NickCommandHandler::announceNicknameChanged(
      DbContext* db, const Client* client, const std::string& old_nickname,
      const std::string& new_nickname, std::vector<int>* fd_list);

 public:
  NickCommandHandler();
  ~NickCommandHandler();
  std::vector<int> operator()(const int fd, const std::string& message);
};

}  // namespace Just1RCe

#endif
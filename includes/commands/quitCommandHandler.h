#ifndef JUST1RCE_INCLUDES_COMMANDS_QUIT_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_QUIT_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class QuitCommandHandler : public CommandHandler {
 private:
  QuitCommandHandler(const QuitCommandHandler &);
  QuitCommandHandler &operator=(const QuitCommandHandler &);
  const int CheckQuit(const Client& client, const std::string& token);

 public:
  QuitCommandHandler();
  ~QuitCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_QUIT_COMMANDHANDLER_H_
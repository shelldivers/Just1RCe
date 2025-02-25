#ifndef JUST1RCE_INCLUDES_COMMANDS_PART_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_PART_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class DbContext;
class Client;

class PartCommandHandler : public Just1RCe::CommandHandler {
 private:
  PartCommandHandler(const PartCommandHandler& rhs);
  PartCommandHandler& operator=(const PartCommandHandler& rhs);

 public:
  PartCommandHandler();
  ~PartCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_PART_COMMAND_HANDLER_H_

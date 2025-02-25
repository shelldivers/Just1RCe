#ifndef JUST1RCE_INCLUDES_COMMANDS_TOPIC_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_TOPIC_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class DbContext;
class Client;

class TopicCommandHandler : public Just1RCe::CommandHandler {
 private:
  TopicCommandHandler(const TopicCommandHandler& rhs);
  TopicCommandHandler& operator=(const TopicCommandHandler& rhs);

 public:
  TopicCommandHandler();
  ~TopicCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_Topic_COMMAND_HANDLER_H_

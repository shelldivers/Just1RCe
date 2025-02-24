#ifndef JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

namespace Just1RCe {

class Channel;
class Client;

class JoinCommandHandler : public CommandHandler {
 private:
  JoinCommandHandler(const JoinCommandHandler&);
  JoinCommandHandler& operator=(const JoinCommandHandler&);
  void JoinChannelWithSetResponse(Client* client, Channel* channel,
                                  std::vector<std::string>& token_stream,
                                  std::vector<int>* fd_list);
  int CheckChannelMode(Channel* channel);

 public:
  JoinCommandHandler();
  ~JoinCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string& message);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_
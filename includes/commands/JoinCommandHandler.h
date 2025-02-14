#ifndef JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_
#define JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_

#include <string>
#include <vector>

#include "../command_handler.h"

class Channel;
class Client;

namespace Just1RCe {

class JoinCommandHandler : public CommandHandler {
 private:
  JoinCommandHandler(const JoinCommandHandler &);
  JoinCommandHandler &operator=(const JoinCommandHandler &);

 public:
  JoinCommandHandler();
  ~JoinCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
  const int checkChannelMask(Channel &channel);
  const int checkChannelFull(Channel &channel, const size_t client_num);
  const int checkChannelKey(Channel &channel, const std::string &key);
  const int checkInviteOnly(Channel &channel, const int client_fd);
  bool isAlreadyJoined(const std::vector<Client *> &clients,
                       const int client_fd);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMANDS_JOIN_COMMAND_HANDLER_H_
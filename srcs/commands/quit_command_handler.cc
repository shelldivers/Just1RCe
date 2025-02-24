#include "../../includes/commands/quit_command_handler.h"

#include <string.h>

#include <vector>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

namespace Just1RCe {

QuitCommandHandler::QuitCommandHandler() {}

QuitCommandHandler::~QuitCommandHandler() {}

/**
 * @brief QUIT command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 */
std::vector<int> QuitCommandHandler::operator()(const int client_fd,
                                                const std::string &message) {
  Client *client = ContextHolder::GetInstance()->db()->GetClient(client_fd);

  if (client == nullptr || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get token
  std::string token;
  parser.ParseCommandQuit(&token);

  // send QUIT message to all clients in the channels
  std::vector<int> fd_list;
  std::vector<Channel *> channels =
      ContextHolder::GetInstance()->db()->GetChannelsByClientFd(client_fd);
  for (int channel_index = 0; channel_index < channels.size();
       ++channel_index) {
    std::string response = ":" + client->GetNickname() + " QUIT :" + token;
    std::vector<Client *> clients =
        ContextHolder::GetInstance()->db()->GetClientsByChannelName(
            channels[channel_index]->name());

    for (int client_index = 0; client_index < channel_fd_list.size();
         ++client_index) {
      if (clients[client_index]->GetFd() == client_fd) {
        continue;
      }
      clients[client_index]->SetSendMessage(response);
      fd_list.push_back(channel_fd);
    }
  }
  // Delete client from server
  db->DelClient(client_fd);

  return fd_list;
}

}  // namespace Just1RCe
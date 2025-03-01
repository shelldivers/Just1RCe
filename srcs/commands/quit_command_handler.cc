#include "../../includes/commands/quit_command_handler.h"

#include <string>
#include <vector>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

namespace Just1RCe {

void BroadcastQuit(Client *client, Channel *channel, const std::string &token,
                   std::vector<int> *fd_list);

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
  DbContext *db = ContextHolder::GetInstance()->db();
  Client *client = db->GetClient(client_fd);

  if (client == NULL || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get token
  std::string token;
  Parser parser(message);
  parser.ParseCommandQuit(&token);

  std::vector<int> fd_list;
  std::vector<Channel *> channels =
      ContextHolder::GetInstance()->db()->GetChannelsByClientFd(client_fd);
  for (size_t channel_index = 0; channel_index < channels.size();
       ++channel_index) {
    // Send QUIT message to all clients in the channel
    BroadcastQuit(client, channels[channel_index], token, &fd_list);

    // Part from channel
    ContextHolder::GetInstance()->db()->PartClientFromChannel(
        client_fd, channels[channel_index]->name());
  }
  // Delete client from server
  db->DelClient(client_fd);

  return fd_list;
}

void BroadcastQuit(Client *client, Channel *channel, const std::string &token,
                   std::vector<int> *fd_list) {
  const std::string client_fullname = client->nick_name() + "!" +
                                      client->user_name() + "@" +
                                      client->GetHostName();
  const std::string response = ":" + client->nick_name() + " QUIT :" + token;
  std::vector<Client *> clients =
      ContextHolder::GetInstance()->db()->GetClientsByChannelName(
          channel->name());

  for (size_t client_index = 0; client_index < clients.size(); ++client_index) {
    if (clients[client_index]->nick_name() == client->nick_name()) {
      continue;
    }
    clients[client_index]->SetSendMessage(response);
    fd_list->push_back(clients[client_index]->GetFd());
  }
}

}  // namespace Just1RCe
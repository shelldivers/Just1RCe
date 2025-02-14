#include "../../includes/commands/quitCommandHandler.h"

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
 * @brief PING command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 */
std::vector<int> QuitCommandHandler::operator()(const int client_fd,
                                                const std::string &message) {
  Parser parser(message);
  DbContext *db = ContextHolder::GetInstance()->db;
  Client *client = db->GetClient(client_fd);
  std::vector<int> fd_list;

  if (client == nullptr) {
    return fd_list;
  }

  std::string token;
  parser.ParseCommandQuit(&token);

  std::vector<Channel *> channels = db->GetChannelsByClientFd(client_fd);
  for (int channel_index = 0; channel_index < channels.size();
       ++channel_index) {
    std::string response = ":" + client->GetNickname() + " QUIT :" + token;
    std::vector<Client *> clients =
        db->GetClientsByChannelName(channels[channel_index]->name());

    for (int client_index = 0; client_index < channel_fd_list.size();
         ++client_index) {
      clients[client_index]->SetSendMessage(response);
      fd_list.push_back(channel_fd);
    }
  }
  db->DelClient(client_fd);

  return fd_list;
}

}  // namespace Just1RCe
#include "../../includes/commands/nickCommandHandler.h"

#include <string.h>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

static const int checkNicknameForbidden(const std::string& nickname);
static const int announceNicknameChanged();

namespace Just1RCe {

NickCommandHandler::NickCommandHandler() {}

NickCommandHandler::~NickCommandHandler() {}

/**
 * @brief NICK command handler
 *
 * @param fd: client fd
 * @param message: message from client
 * @return std::vector<int>: list of fds to send message
 *
 * @details
 * Get 3 types of numeric error code
 * - ERR_NONICKNAMEGIVEN(431): No nickname given
 * - ERR_ERRONEUSNICKNAME(432): Erroneous Nickname
 * - ERR_NICKNAMEINUSE(433): Nickname is already in use
 */
std::vector<int> NickCommandHandler::operator()(const int client_fd,
                                                const std::string& message) {
  Parser parser(message);
  DbContext* db = ContextHolder::GetInstance()->db;
  Client* client = db->GetClient(client_fd);
  std::vector<int> fd_list;

  if (!client) {
    return fd_list;
  }

  std::string new_nickname;
  std::string old_nickname = client->nick_name();

  if (parser.ParseCommandNick(&new_nickname) == ERR_NONICKNAMEGIVEN) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 431 " + old_nickname +
                           " :No nickname given");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  if (checkNicknameForbidden(new_nickname) == ERR_ERRONEUSNICKNAME) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 432 " + new_nickname +
                           " :Erroneous Nickname");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  if (db->GetClientByNickname(new_nickname) != nullptr) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 433 " + old_nickname +
                           " " + new_nickname +
                           " : Nickname is already in use.");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  if (old_nickname.empty()) {
    client->set_nick_name(new_nickname);

    return fd_list;
  }

  announceNicknameChanged(db->GetChannelsByClientFd(client->GetFd()),
                          old_nickname, new_nickname, client, &fd_list);

  return fd_list;
}

const int checkNicknameForbidden(const std::string& nickname) {
  const std::string forbidden_chars = "&#$: ,*?!@";

  for (size_t nickname_index = 0; nickname_index < nickname.size();
       nickname_index++) {
    if (forbidden_chars.find(nickname[nickname_index]) != std::string::npos) {
      return ERR_ERRONEUSNICKNAME;
    }
  }

  return IRC_NOERROR;
}

const int announceNicknameChanged(const std::vector<Channel*>& channels,
                                  const std::string& old_nickname,
                                  const std::string& new_nickname,
                                  Client* client, std::vector<int>* fd_list) {
  const std::string nickname_changed = old_nickname + " NICK " + new_nickname;

  client->SetSendMessage(nickname_changed);
  fd_list->push_back(client->GetFd());

  for (size_t channel_index = 0; channel_index < channels.size();
       channel_index++) {
    std::vector<Client*> clients =
        db->GetClientsByChannelName(channels[channel_index]->name());

    for (size_t client_index = 0; client_index < clients.size();
         client_index++) {
      if (clients[client_index] == client) {
        continue;
      }

      clients[client_index]->SetSendMessage(nickname_changed);
      fd_list->push_back(clients[client_index]->GetFd());
    }
  }

  return IRC_NOERROR;
}

}  // namespace Just1RCe

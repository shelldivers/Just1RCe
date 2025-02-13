#include "../../includes/commands/nickCommandHandler.h"

#include <string.h>

#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

namespace Just1RCe {

NickCommandHandler::NickCommandHandler() {}

NickCommandHandler::~NickCommandHandler() {}

std::vector<int> NickCommandHandler::operator()(const int fd,
                                                const std::string& message) {
  Parser parser(message);
  DbContext* db = ContextHolder::GetInstance()->db;
  Client* client = db->GetClient(fd);
  std::vector<int> fd_list;

  if (!client) {
    return fd_list;
  }

  std::string new_nickname;
  std::string old_nickname = client->nick_name();

  int numeric = parser.ParseCommandNick(&new_nickname);
  if (numeric) {
    client->SetSendMessage(":" + SERVER_NAME + " 431 " + old_nickname +
                           " :No nickname given");
    fd_list.push_back(fd);

    return fd_list;
  }

  if (old_nickname.empty()) {
    client->set_nick_name(new_nickname);

    return fd_list;
  }

  numeric = checkNicknameForbidden(new_nickname);
  if (numeric) {
    client->SetSendMessage(":" + SERVER_NAME + " 432 " + new_nickname +
                           " :Erroneous Nickname");
    fd_list.push_back(fd);

    return fd_list;
  }

  if (db->GetClientByNickname(new_nickname) != nullptr) {
    client->SetSendMessage(":" + SERVER_NAME + " 433 " + old_nickname + " " +
                           new_nickname + " : Nickname is already in use.");
    fd_list.push_back(fd);

    return fd_list;
  }

  announceNicknameChanged(db, client, old_nickname, new_nickname, &fd_list);

  return fd_list;
}

const int NickCommandHandler::checkNicknameForbidden(
    const std::string& nickname) {
  const std::string forbidden_chars = "&#$: ,*?!@";

  for (size_t nickname_index = 0; nickname_index < nickname.size();
       nickname_index++) {
    if (forbidden_chars.find(nickname[nickname_index]) != std::string::npos) {
      return ERR_ERRONEUSNICKNAME;
    }
  }

  return IRC_NOERROR;
}

const int NickCommandHandler::announceNicknameChanged(
    DbContext* db, Client* client, const std::string& old_nickname,
    const std::string& new_nickname, std::vector<int>* fd_list) {
  std::vector<Channel*> channels = db->GetChannelsByClientFd(client->GetFd());
  const std::string nickname_changed = old_nickname + " NICK " + new_nickname;

  client->SetSendMessage(nickname_changed);
  fd_list->push_back(client->GetFd());

  if (channels.empty()) {
    return IRC_NOERROR;
  }

  for (size_t channel_index = 0; channel_index < channels.size();
       channel_index++) {
    std::vector<Client*> clients =
        db->GetClientsByChannelName(channels[channel_index]->name());

    for (size_t client_index = 0; client_index < clients.size(); client_index++) {
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

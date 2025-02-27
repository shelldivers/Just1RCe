#include "../../includes/commands/nick_command_handler.h"

#include <string>
#include <vector>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"
#include "../../includes/response_generator.h"

static bool IsForbidden(const std::string& nickname);

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
  Client* client = ContextHolder::GetInstance()->db()->GetClient(client_fd);

  if (client == NULL || client->IsPassed() == false) {
    return std::vector<int>();
  }

  // Parse
  std::string new_nickname;
  std::string old_nickname = client->nick_name();
  Parser parser(message);
  parser.ParseCommandNick(&new_nickname);

  // Check error
  int numeric = CheckNick(new_nickname);
  if (numeric != IRC_NOERROR) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        numeric,
        ResponseArguments(numeric, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  client->set_nick_name(new_nickname);

  // Send response
  // 1. If the client is not authenticated, send welcome message
  // 2. If the client is in channels, send NICK message to all clients in the
  // channels
  if (old_nickname.empty() == true && client->IsAuthenticated() == true) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        RPL_WELCOME,
        ResponseArguments(RPL_WELCOME, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  } else if (old_nickname.empty() == false) {
    std::vector<int> fd_list;
    AnnounceNickChanged(client, old_nickname, new_nickname, &fd_list);

    return fd_list;
  }

  return std::vector<int>();
}

void NickCommandHandler::AnnounceNickChanged(Client* client,
                                             const std::string& old_nickname,
                                             const std::string& new_nickname,
                                             std::vector<int>* fd_list) {
  std::vector<Channel*> channels =
      ContextHolder::GetInstance()->db()->GetChannelsByClientFd(
          client->GetFd());
  const std::string nickname_changed = old_nickname + " NICK " + new_nickname;

  for (size_t channel_index = 0; channel_index < channels.size();
       channel_index++) {
    std::vector<Client*> clients =
        ContextHolder::GetInstance()->db()->GetClientsByChannelName(
            channels[channel_index]->name());

    for (size_t client_index = 0; client_index < clients.size();
         client_index++) {
      if (clients[client_index] == client) {
        continue;
      }

      clients[client_index]->SetSendMessage(nickname_changed);
      fd_list->push_back(clients[client_index]->GetFd());
    }
  }
  client->SetSendMessage(nickname_changed);
  fd_list->push_back(client->GetFd());
}

/**
 * @brief check error
 *
 * @param client: client
 * @param new_nickname: new nickname
 *
 * @return int: error code
 *
 * @details
 * Check if the new nickname is valid
 * - ERR_NONICKNAMEGIVEN(431): No nickname given
 * - ERR_ERRONEUSNICKNAME(432): Erroneous Nickname
 * - ERR_NICKNAMEINUSE(433): Nickname is already in use
 */
int NickCommandHandler::CheckNick(const std::string& new_nickname) {
  if (new_nickname.empty() == true) {
    return ERR_NONICKNAMEGIVEN;
  }

  if (IsForbidden(new_nickname) == true) {
    return ERR_ERRONEUSNICKNAME;
  }

  if (ContextHolder::GetInstance()->db()->GetFdByNickName(new_nickname) != -1) {
    return ERR_NICKNAMEINUSE;
  }

  return IRC_NOERROR;
}

}  // namespace Just1RCe

bool IsForbidden(const std::string& nickname) {
  const std::string forbidden_chars = "&#$: ,*?!@";

  for (size_t nickname_index = 0; nickname_index < nickname.size();
       nickname_index++) {
    if (forbidden_chars.find(nickname[nickname_index]) != std::string::npos) {
      return true;
    }
  }

  return false;
}

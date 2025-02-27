#include "../../includes/commands/privmsg_command_handler.h"

#include <algorithm>
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

namespace Just1RCe {

static bool IsUserOnChannel(const int client_fd, const Channel &channel);
static void ResponseAsNumeric(Client *client,
                              const std::vector<std::string> &token_stream,
                              int numeric);
static void SentPrivmsgToClient(Client *client, Client *target,
                                const std::string &message,
                                std::vector<int> *fd_list);
static void SendPrivmsgToChannel(const Client &client, const Channel &channel,
                                 const std::string &message,
                                 std::vector<int> *fd_list);

PrivmsgCommandHandler::PrivmsgCommandHandler() {}

PrivmsgCommandHandler::~PrivmsgCommandHandler() {}

/**
 * @brief Privmsg command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * ERR_NOSUCHNICK (401): No such nick
 * ERR_NORECIPIENT (411): No recipient given
 * ERR_NOTEXTTOSEND (412): No text to send
 * ERR_CANNOTSENDTOCHAN (404): Cannot send to channel
 */
std::vector<int> PrivmsgCommandHandler::operator()(const int client_fd,
                                                   const std::string &message) {
  DbContext *db = ContextHolder::GetInstance()->db();
  Client *client = db->GetClient(client_fd);

  if (client == NULL || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get token
  Parser parser(message);
  std::vector<std::string> token_stream = parser.GetTokenStream();

  // ERR_NORECIPIENT (411): No recipient given
  if (token_stream.size() < 2) {
    ResponseAsNumeric(client, token_stream, ERR_NORECIPIENT);
    return std::vector<int>(1, client_fd);
  }

  // ERR_NOTEXTTOSEND (412): No text to send
  if (token_stream.size() < 3) {
    ResponseAsNumeric(client, token_stream, ERR_NOTEXTTOSEND);
    return std::vector<int>(1, client_fd);
  }

  std::vector<int> fd_list;
  std::string target_name = token_stream[1];
  std::string message_to_send = token_stream[2];

  // ERR_CANNOTSENDTOCHAN (404): Cannot send to channel
  if (target_name[0] == '#' || target_name[0] == '&') {
    Channel *channel = db->GetChannel(target_name);
    if (channel == NULL || IsUserOnChannel(client_fd, *channel) == false) {
      ResponseAsNumeric(client, token_stream, ERR_CANNOTSENDTOCHAN);
      return std::vector<int>(1, client_fd);
    }
    // Send message to channel
    SendPrivmsgToChannel(*client, *channel, message_to_send, &fd_list);
  } else {
    int target_fd = db->GetFdByNickName(target_name);
    // ERR_NOSUCHNICK (401): No such nick
    if (target_fd == -1) {
      ResponseAsNumeric(client, token_stream, ERR_NOSUCHNICK);
      return std::vector<int>();
    }

    Client *target = db->GetClient(target_fd);
    // Send message to target
    SentPrivmsgToClient(client, target, message_to_send, &fd_list);
  }
  return fd_list;
}

static bool IsUserOnChannel(const int client_fd, const Channel &channel) {
  DbContext *db = ContextHolder::GetInstance()->db();
  std::vector<Channel *> channels = db->GetChannelsByClientFd(client_fd);

  if (std::find(channels.begin(), channels.end(), &channel) != channels.end()) {
    return true;
  }

  return false;
}

static void SendPrivmsgToChannel(const Client &client, const Channel &channel,
                                 const std::string &message,
                                 std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();
  std::vector<Client *> clients = db->GetClientsByChannelName(channel.name());
  std::string client_fullname = client.nick_name() + "!" + client.user_name() +
                                "@" + client.GetHostName();
  std::string response =
      ":" + client_fullname + " PRIVMSG " + channel.name() + " :" + message;

  for (size_t index = 0; index < clients.size(); ++index) {
    if (client.GetFd() == clients[index]->GetFd()) {
      continue;
    }
    Client *client = clients[index];
    client->SetSendMessage(response);
    fd_list->push_back(client->GetFd());
  }
}

static void SentPrivmsgToClient(Client *client, Client *target,
                                const std::string &message,
                                std::vector<int> *fd_list) {
  std::string response = ":" + client->nick_name() + " PRIVMSG " +
                         target->nick_name() + " :" + message;
  target->SetSendMessage(response);
  fd_list->push_back(target->GetFd());
}

/*===========Response functions (overloaded)===========*/

static void ResponseAsNumeric(Client *client,
                              const std::vector<std::string> &token_stream,
                              int numeric) {
  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response = generator.GenerateResponse(
      numeric, ResponseArguments(numeric, *client, NULL, token_stream));
  client->SetSendMessage(response);
}

}  // namespace Just1RCe
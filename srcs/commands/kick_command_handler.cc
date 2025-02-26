#include "../../includes/commands/kick_command_handler.h"

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
static void ResponseAsNumeric(Client *client, const Channel *channel,
                              const std::vector<std::string> &token_stream,
                              int numeric);
static void ResponseAsNumeric(Client *client,
                              const std::vector<std::string> &token_stream,
                              int numeric);

KickCommandHandler::KickCommandHandler() {}

KickCommandHandler::~KickCommandHandler() {}

/**
 * @brief Kick command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * RPL_INVITING (341): Kick is sent to the client
 * ERR_NEEDMOREPARAMS (461): Not enough parameters
 * ERR_NOSUCHCHANNEL (403): No such channel
 * ERR_CHANOPRIVSNEEDED (482): You're not channel operator
 * ERR_USERNOTINCHANNEL (441): Target not in channel
 * ERR_NOTONCHANNEL (442): Client is not on channel
 */
std::vector<int> KickCommandHandler::operator()(const int client_fd,
                                                const std::string &message) {
  DbContext *db = ContextHolder::GetInstance()->db();
  Client *client = db->GetClient(client_fd);

  if (client == NULL || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get token
  Parser parser(message);
  std::vector<std::string> token_stream = parser.GetTokenStream();

  // ERR_NEEDMOREPARAMS (461): Not enough parameters
  if (token_stream.size() < 3) {
    ResponseAsNumeric(client, token_stream, ERR_NEEDMOREPARAMS);
    return std::vector<int>(1, client_fd);
  }

  // ERR_NOSUCHCHANNEL (403): No such channel
  std::string channel_name = token_stream[1];
  Channel *channel = db->GetChannel(channel_name);
  if (channel == NULL) {
    ResponseAsNumeric(client, token_stream, ERR_NOSUCHCHANNEL);
    return std::vector<int>(1, client_fd);
  }

  // ERR_CHANOPRIVSNEEDED (482): You're not channel operator
  if (channel->GetOperatorFd() != client_fd) {
    ResponseAsNumeric(client, channel, token_stream, ERR_CHANOPRIVSNEEDED);
    return std::vector<int>(1, client_fd);
  }

  // ERR_NOTONCHANNEL (442): Client is not on channel
  if (IsUserOnChannel(client_fd, *channel) == false) {
    ResponseAsNumeric(client, channel, token_stream, ERR_NOTONCHANNEL);
    return std::vector<int>(1, client_fd);
  }

  // ERR_USERNOTINCHANNEL (441): Target not in channel
  std::string target_name = token_stream[2];
  int target_fd = db->GetFdByNickName(target_name);
  if (IsUserOnChannel(target_fd, *channel) == false) {
    ResponseAsNumeric(client, channel, token_stream, ERR_USERNOTINCHANNEL);
    return std::vector<int>(1, client_fd);
  }

  // Kick target from channel
  // Kick is sent to the clients
  std::vector<int> fd_list;
  std::string reason = "For some reason";
  if (token_stream.size() > 3) {
    reason = token_stream[3];
  }
  std::string client_full_name = client->nick_name() + "!" +
                                 client->user_name() + "@" +
                                 client->GetHostName();
  std::string response = ":" + client_full_name + " KICK " + channel_name +
                         " " + target_name + " :" + reason;

  std::vector<Client *> clients = db->GetClientsByChannelName(channel_name);
  for (size_t index = 0; index < clients.size(); index++) {
    clients[index]->SetSendMessage(response);
    fd_list.push_back(clients[index]->GetFd());
  }
  db->PartClientFromChannel(target_fd, channel_name);

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

/*===========Response functions (overloaded)===========*/

static void ResponseAsNumeric(Client *client, const Channel *channel,
                              const std::vector<std::string> &token_stream,
                              int numeric) {
  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response = generator.GenerateResponse(
      numeric, ResponseArguments(numeric, *client, channel, token_stream));
  client->SetSendMessage(response);
}

static void ResponseAsNumeric(Client *client,
                              const std::vector<std::string> &token_stream,
                              int numeric) {
  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response = generator.GenerateResponse(
      numeric, ResponseArguments(numeric, *client, NULL, token_stream));
  client->SetSendMessage(response);
}

}  // namespace Just1RCe
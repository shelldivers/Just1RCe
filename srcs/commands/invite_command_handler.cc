#include "../../includes/commands/invite_command_handler.h"

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
                              std::vector<int> *fd_list, int numeric);
static void ResponseAsNumeric(Client *client, const Channel *channel,
                              const std::vector<std::string> &token_stream,
                              int numeric);
static void ResponseAsNumeric(Client *client,
                              const std::vector<std::string> &token_stream,
                              int numeric);

InviteCommandHandler::InviteCommandHandler() {}

InviteCommandHandler::~InviteCommandHandler() {}

/**
 * @brief Invite command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * RPL_INVITING (341): Invite is sent to the client
 * ERR_NEEDMOREPARAMS (461): Not enough parameters
 * ERR_NOSUCHNICK (401): No such nick
 * ERR_NOSUCHCHANNEL (403): No such channel
 * ERR_NOTONCHANNEL (442): Client is not on channel
 * ERR_USERONCHANNEL (443): User is already on channel
 */
std::vector<int> InviteCommandHandler::operator()(const int client_fd,
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
  if (token_stream.size() < 2) {
    ResponseAsNumeric(client, token_stream, ERR_NEEDMOREPARAMS);
    return std::vector<int>(1, client_fd);
  }

  std::string target_nick = token_stream[1];
  int target_client_fd = db->GetFdByNickName(target_nick);

  // ERR_NOSUCHNICK (401): No such nick
  if (target_client_fd == -1) {
    ResponseAsNumeric(client, token_stream, ERR_NOSUCHNICK);
    return std::vector<int>(1, client_fd);
  }

  std::string channel_name = token_stream[2];
  Channel *channel = db->GetChannel(channel_name);

  // ERR_NOSUCHCHANNEL (403): No such channel
  if (channel == NULL) {
    ResponseAsNumeric(client, token_stream, ERR_NOSUCHCHANNEL);
    return std::vector<int>(1, client_fd);
  }

  // ERR_NOTONCHANNEL (442): Client is not on channel
  if (IsUserOnChannel(client_fd, *channel) == false) {
    ResponseAsNumeric(client, channel, token_stream, ERR_NOTONCHANNEL);
    return std::vector<int>(1, client_fd);
  }
  // ERR_USERONCHANNEL (443): User is already on channel
  if (IsUserOnChannel(target_client_fd, *channel) == true) {
    ResponseAsNumeric(client, channel, token_stream, ERR_USERONCHANNEL);
    return std::vector<int>(1, client_fd);
  }

  // RPL_INVITING (341): Invite is sent to the client
  std::vector<int> fd_list;
  ResponseAsNumeric(client, channel, token_stream, &fd_list, RPL_INVITING);

  // Send invite message to target client
  std::string client_full_name = client->nick_name() + "!" +
                                 client->user_name() + "@" +
                                 client->GetHostName();
  std::string response =
      ":" + client_full_name + " INVITE " + target_nick + " :" + channel_name;
  Client *target_client = db->GetClient(target_client_fd);
  
  target_client->SetSendMessage(response);
  fd_list.push_back(target_client_fd);

  channel->Invite(target_client->GetHostName());

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
                              std::vector<int> *fd_list, int numeric) {
  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response = generator.GenerateResponse(
      numeric, ResponseArguments(numeric, *client, channel, token_stream));
  client->SetSendMessage(response);
  fd_list->push_back(client->GetFd());
}

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
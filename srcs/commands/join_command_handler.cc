#include "../../includes/commands/join_command_handler.h"

#include <algorithm>
#include <iostream>
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

static void BroadcastJoined(Client *client, const Channel &channel,
                            std::vector<int> *fd_list);
static void JoinChannelWithResponse(
    Client *client, Channel *channel,
    const std::vector<std::string> &token_stream, std::vector<int> *fd_list);
static int CheckChannelMode(const Client &client, Channel *channel,
                            const std::string &key);
static bool IsChannelNameValid(const std::string &channel_name);
static void AddChannelAndJoinWithResponse(
    Client *client, const std::string &channel_name,
    const std::vector<std::string> &token_stream, std::vector<int> *fd_list);
static void ResponseAsNumeric(Client *client, Channel *channel,
                              const std::vector<std::string> &token_stream,
                              std::vector<int> *fd_list, int numeric);

JoinCommandHandler::JoinCommandHandler() {}

JoinCommandHandler::~JoinCommandHandler() {}

/**
 * @brief JOIN command handler
 *
 * @param client_fd to identify client that te message originate from
 * @param message to identify client that te message originate from
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * Get 8 types of Numeric Reply
 * - RPL_TOPIC (332)
 * - RPL_NAMREPLY (353)
 * - RPL_ENDOFNAMES (366)
 * - ERR_BADCHANMASK (476)
 * - ERR_CHANNELISFULL (471)
 * - ERR_BADCHANNELKEY (475)
 * - ERR_INVITEONLYCHAN (473)
 * - ERR_NEEDMOREPARAMS (461)
 */
std::vector<int> JoinCommandHandler::operator()(const int client_fd,
                                                const std::string &message) {
  DbContext *db = ContextHolder::GetInstance()->db();
  Client *client = db->GetClient(client_fd);

  if (client == NULL || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get channel names and keys
  std::vector<std::string> channel_names;
  std::vector<std::string> keys;
  Parser parser(message);
  parser.ParseCommandJoin(&channel_names, &keys);
  std::vector<std::string> token_stream = parser.GetTokenStream();

  // No channel recieved
  if (token_stream.size() < 2) {
    ResponseAsNumeric(client, NULL, token_stream, NULL, ERR_NEEDMOREPARAMS);
    return std::vector<int>(1, client_fd);
  }

  std::vector<int> fd_list;
  for (size_t index = 0; index < channel_names.size(); ++index) {
    Channel *channel = db->GetChannel(channel_names[index]);

    if (IsChannelNameValid(channel_names[index]) == false) {
      ResponseAsNumeric(client, channel, token_stream, &fd_list,
                        ERR_BADCHANMASK);
      continue;
    }

    // If channel not exist, create channel
    if (channel == NULL) {
      AddChannelAndJoinWithResponse(client, channel_names[index], token_stream,
                                    &fd_list);
      continue;
    }

    std::string key = "";
    if (index < keys.size()) {
      key = keys[index];
    }
    int numeric = CheckChannelMode(*client, channel, key);
    if (numeric != IRC_NOERROR) {
      ResponseAsNumeric(client, channel, token_stream, &fd_list, numeric);
      continue;
    }
    BroadcastJoined(client, *channel, &fd_list);
    JoinChannelWithResponse(client, channel, token_stream, &fd_list);
  }

  return fd_list;
}

static void BroadcastJoined(Client *client, const Channel &channel,
                            std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  std::vector<Client *> client_list =
      db->GetClientsByChannelName(channel.name());
  std::string response = ":" + client->nick_name() + '!' + client->user_name() +
                         '@' + client->GetHostName() + " JOIN " +
                         channel.name();
  for (size_t index = 0; index < client_list.size(); ++index) {
    client_list[index]->SetSendMessage(response);
    fd_list->push_back(client_list[index]->GetFd());
  }
}

static void JoinChannelWithResponse(
    Client *client, Channel *channel,
    const std::vector<std::string> &token_stream, std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  db->JoinClientToChannel(client->GetFd(), channel->name());

  // Topic message
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_TOPIC_OPRT_ONLY) &&
      channel->topic().empty() == false) {
    ResponseAsNumeric(client, channel, token_stream, fd_list, RPL_TOPIC);
  }
  // NameReply
  ResponseAsNumeric(client, channel, token_stream, fd_list, RPL_NAMREPLY);
}

static int CheckChannelMode(const Client &client, Channel *channel,
                            const std::string &key) {
  // Invite only
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY) &&
      channel->IsInvited(client.user_name() + "@" + client.GetHostName()) ==
          false) {
    return ERR_INVITEONLYCHAN;
  }
  // Private channel
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET) &&
      channel->key() != key) {
    return ERR_BADCHANNELKEY;
  }
  // Maximun client number
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT) &&
      channel->cur_user_count() >= channel->max_user_num()) {
    return ERR_CHANNELISFULL;
  }

  return IRC_NOERROR;
}

static bool IsChannelNameValid(const std::string &channel_name) {
  if (channel_name.size() <= 1) {
    return false;
  }
  if (channel_name[0] != '#' && channel_name[0] != '&') {
    return false;
  }
  for (size_t index = 1; index < channel_name.size(); ++index) {
    if (channel_name[index] == ' ' || channel_name[index] == '\a' ||
        channel_name[index] == ',') {
      return false;
    }
  }

  return true;
}

static void AddChannelAndJoinWithResponse(
    Client *client, const std::string &channel_name,
    const std::vector<std::string> &token_stream, std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  Channel *channel = new Channel(channel_name);
  channel->operator_fd(client->GetFd());
  db->AddChannel(channel);
  JoinChannelWithResponse(client, channel, token_stream, fd_list);
}

static void ResponseAsNumeric(Client *client, Channel *channel,
                              const std::vector<std::string> &token_stream,
                              std::vector<int> *fd_list, int numeric) {
  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response = generator.GenerateResponse(
      numeric, ResponseArguments(numeric, *client, channel, token_stream));
  client->SetSendMessage(response);
  if (fd_list != NULL) {
    fd_list->push_back(client->GetFd());
  }
}

}  // namespace Just1RCe

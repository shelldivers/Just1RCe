#include "../../includes/commands/join_command_handler.h"

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

void JoinChannelWithResponse(Client *client, Channel *channel,
                             const std::vector<std::string> &token_stream,
                             std::vector<int> *fd_list);
void PartFromAllChannelsWithResponse(
    Client *client, const std::vector<std::string> &channel_names,
    std::vector<int> *fd_list);
int CheckChannel(const Client &client, Channel *channel,
                 const std::string &key);
void AnnounceJoined(Client *client, const Channel &channel,
                    std::vector<int> *fd_list);

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
 * - RPL_NOTOPIC (331)
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

  if (channel_names.size() == 1 && channel_names[0] == "0") {
    std::vector<int> fd_list;
    PartFromAllChannelsWithResponse(client, channel_names, &fd_list);

    return fd_list;
  }

  std::vector<int> fd_list;
  for (size_t index = 0; index < channel_names.size(); ++index) {
    Channel *channel = db->GetChannel(channel_names[index]);

    // 채널이 없으면 채널을 새로 생성하고 JOIN
    if (channel == NULL) {
      channel = new Channel(channel_names[index]);
      db->AddChannel(channel);
      JoinChannelWithResponse(client, channel, parser.GetTokenStream(),
                              &fd_list);
      continue;
    }

    std::string key = "";
    if (index < keys.size()) {
      key = keys[index];
    }
    int numeric = CheckChannel(*client, channel, key);
    if (numeric != IRC_NOERROR) {
      ResponseGenerator &generator = ResponseGenerator::GetInstance();
      std::string response = generator.GenerateResponse(
          numeric, ResponseArguments(numeric, *client, channel,
                                     parser.GetTokenStream()));

      client->SetSendMessage(response);
      fd_list.push_back(client_fd);
      continue;
    }

    // Announce JOIN
    AnnounceJoined(client, *channel, &fd_list);
    JoinChannelWithResponse(client, channel, parser.GetTokenStream(), &fd_list);
  }

  return fd_list;
}

void AnnounceJoined(Client *client, const Channel &channel,
                    std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  std::vector<Client *> client_list =
      db->GetClientsByChannelName(channel.name());
  std::string response = client->nick_name() + " JOIN :" + channel.name();
  for (size_t index = 0; index < client_list.size(); ++index) {
    client_list[index]->SetSendMessage(response);
    fd_list->push_back(client_list[index]->GetFd());
  }
}

void JoinChannelWithResponse(Client *client, Channel *channel,
                             const std::vector<std::string> &token_stream,
                             std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  db->JoinClientToChannel(client->GetFd(), channel->name());

  ResponseGenerator &generator = ResponseGenerator::GetInstance();
  std::string response;
  // Topic message
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_TOPIC_OPRT_ONLY)) {
    response = generator.GenerateResponse(
        RPL_TOPIC,
        ResponseArguments(RPL_TOPIC, *client, channel, token_stream));
    client->SetSendMessage(response);
    fd_list->push_back(client->GetFd());
  }
  // NameReply
  response = generator.GenerateResponse(
      RPL_NAMREPLY,
      ResponseArguments(RPL_NAMREPLY, *client, channel, token_stream));
  client->SetSendMessage(response);
  fd_list->push_back(client->GetFd());
}

void PartFromAllChannelsWithResponse(
    Client *client, const std::vector<std::string> &channel_names,
    std::vector<int> *fd_list) {
  DbContext *db = ContextHolder::GetInstance()->db();

  // Loop channel list
  for (size_t index = 0; index < channel_names.size(); ++index) {
    std::vector<Client *> client_list =
        db->GetClientsByChannelName(channel_names[index]);
    std::string response = client->nick_name() + " PART";

    // Part channel
    db->PartClientFromChannel(client->GetFd(), channel_names[index]);

    // Send message
    for (size_t client_index = 0; client_index < client_list.size();
         ++client_index) {
      client_list[client_index]->SetSendMessage(response);
      fd_list->push_back(client_list[client_index]->GetFd());
    }
  }
}

int CheckChannel(const Client &client, Channel *channel,
                 const std::string &key) {
  // Channel name valid
  std::string channel_name = channel->name();
  if (channel_name[0] != '@' && channel_name[0] != '&') {
    return ERR_BADCHANMASK;
  }
  for (size_t index = 1; index < channel_name.size(); ++index) {
    if (channel_name[index] == ' ' || channel_name[index] == '\a' ||
        channel_name[index] == ',') {
      return ERR_BADCHANMASK;
    }
  }
  // Invite only
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY &&
                         channel->IsInvited(client.GetHostName()) == false)) {
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

}  // namespace Just1RCe
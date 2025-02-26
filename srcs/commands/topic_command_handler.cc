#include "../../includes/commands/topic_command_handler.h"

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

static int CheckNumericError(const Client& client, const Channel* channel);
static void BroadcastTopic(const Channel* channel, std::vector<int>* fd_list);

TopicCommandHandler::TopicCommandHandler() {}

TopicCommandHandler::~TopicCommandHandler() {}

/**
 * @brief Topic command handler
 *
 * @param fd: client fd
 * @param message: message from client
 * @return std::vector<int>: list of fds to send message
 *
 * @details
 * Get 3 types of numeric code
 * - RPL_TOPIC (332): Topic message
 * - ERR_NEEDMOREPARAMS (461): Need more parameters
 * - ERR_NOSUCHCHANNEL (403): No such channel
 * - ERR_NOTONCHANNEL (442): Client not on channel
 * - ERR_CHANOPRIVSNEEDED (482): Client not channel operator
 * - RPL_NOTOPIC (331): No Topic message
 */
std::vector<int> TopicCommandHandler::operator()(const int client_fd,
                                                 const std::string& message) {
  DbContext* db = ContextHolder::GetInstance()->db();
  ResponseGenerator& generator = ResponseGenerator::GetInstance();

  Client* client = db->GetClient(client_fd);
  if (client == NULL || client->IsPassed() == false) {
    return std::vector<int>();
  }

  // Parse
  Parser parser(message);
  std::vector<std::string> token_stream = parser.GetTokenStream();
  if (token_stream.size() < 2) {
    return std::vector<int>(1, client_fd);
  }

  std::string channel_name = token_stream[1];
  Channel* channel = db->GetChannel(channel_name);
  int numeric = CheckNumericError(*client, channel);
  if (numeric != IRC_NOERROR) {
    std::string response = generator.GenerateResponse(
        numeric, ResponseArguments(numeric, *client, channel, token_stream));
    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Get topic or no topic
  // RPL_TOPIC (332): Topic message
  // RPL_NOTOPIC (331): No Topic message
  numeric = RPL_TOPIC;
  if (token_stream.size() == 2) {
    if (channel->topic().empty()) {
      numeric = RPL_NOTOPIC;
    }
    std::string response = generator.GenerateResponse(
        numeric, ResponseArguments(numeric, *client, channel, token_stream));
    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Set topic
  std::vector<int> fd_list;
  std::string topic = token_stream[2];
  channel->set_topic(topic);
  BroadcastTopic(channel, &fd_list);

  return std::vector<int>();
}

static int CheckNumericError(const Client& client, const Channel* channel) {
  DbContext* db = ContextHolder::GetInstance()->db();

  // ERR_NOSUCHCHANNEL (403): No such channel
  if (channel == NULL) {
    return ERR_NOSUCHCHANNEL;
  }

  // ERR_NOTONCHANNEL (442): Client not on channel
  std::vector<Client*> clients = db->GetClientsByChannelName(channel->name());
  if (std::find(clients.begin(), clients.end(), &client) == clients.end()) {
    return ERR_NOTONCHANNEL;
  }

  // ERR_CHANOPRIVSNEEDED (482): Client not channel operator
  if (channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_TOPIC_OPRT_ONLY) &&
      channel->GetOperatorFd() != client.GetFd()) {
    return ERR_CHANOPRIVSNEEDED;
  }

  return IRC_NOERROR;
}

static void BroadcastTopic(const Channel* channel, std::vector<int>* fd_list) {
  DbContext* db = ContextHolder::GetInstance()->db();
  std::string response = "TOPIC " + channel->name() + " :" + channel->topic();

  std::vector<Client*> clients = db->GetClientsByChannelName(channel->name());
  for (size_t index = 0; index < clients.size(); ++index) {
    clients[index]->SetSendMessage(response);
    fd_list->push_back(clients[index]->GetFd());
  }
}

}  // namespace Just1RCe

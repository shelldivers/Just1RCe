#include "../../includes/commands/part_command_handler.h"

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

static void BroadcastPart(const std::string& channel_name,
                          const std::string& client_name,
                          std::vector<int>* fd_list);
static bool IsClientExistOnChannel(const int client_fd,
                                   const std::string& channel_name);
static bool IsChannelExist(const std::string& channel_name);

PartCommandHandler::PartCommandHandler() {}

PartCommandHandler::~PartCommandHandler() {}

/**
 * @brief Part command handler
 *
 * @param fd: client fd
 * @param message: message from client
 * @return std::vector<int>: list of fds to send message
 *
 * @details
 * Get 2 types of numeric error code
 * ERR_NEEDMOREPARAMS (461)
 * ERR_NOTONCHANNEL (442)
 * ERR_NOSUCHCHANNEL (403)
 */
std::vector<int> PartCommandHandler::operator()(const int client_fd,
                                                const std::string& message) {
  DbContext* db = ContextHolder::GetInstance()->db();
  Client* client = db->GetClient(client_fd);

  if (client == NULL || client->IsPassed() == false) {
    return std::vector<int>();
  }

  std::vector<std::string> channel_names;
  Parser parser(message);
  parser.ParseCommandPart(&channel_names);

  // Check parameter exist
  if (channel_names.size() == 0 || channel_names[0].empty() == true) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        ERR_NEEDMOREPARAMS, ResponseArguments(ERR_NEEDMOREPARAMS, *client, NULL,
                                              parser.GetTokenStream()));

    return std::vector<int>(1, client_fd);
  }

  std::vector<int> fd_list;
  for (size_t index = 0; index < channel_names.size(); ++index) {
    // Check channel exist
    if (IsChannelExist(channel_names[index]) == false) {
      ResponseGenerator& generator = ResponseGenerator::GetInstance();
      std::string response = generator.GenerateResponse(
          ERR_NOSUCHCHANNEL, ResponseArguments(ERR_NOSUCHCHANNEL, *client, NULL,
                                               parser.GetTokenStream()));

      client->SetSendMessage(response);
      return std::vector<int>(1, client_fd);
    }

    // Check client exist
    if (IsClientExistOnChannel(client_fd, channel_names[index]) == false) {
      ResponseGenerator& generator = ResponseGenerator::GetInstance();
      std::string response = generator.GenerateResponse(
          ERR_NOTONCHANNEL, ResponseArguments(ERR_NOTONCHANNEL, *client, NULL,
                                              parser.GetTokenStream()));

      client->SetSendMessage(response);
      return std::vector<int>(1, client_fd);
    }

    BroadcastPart(channel_names[index], client->nick_name(), &fd_list);
    db->PartClientFromChannel(client_fd, channel_names[index]);
  }

  return fd_list;
}

static bool IsChannelExist(const std::string& channel_name) {
  DbContext* db = ContextHolder::GetInstance()->db();
  Channel* channel = db->GetChannel(channel_name);

  if (channel) {
    return true;
  }

  return false;
}

static bool IsClientExistOnChannel(const int client_fd,
                                   const std::string& channel_name) {
  DbContext* db = ContextHolder::GetInstance()->db();
  std::vector<Channel*> channels = db->GetChannelsByClientFd(client_fd);

  for (size_t index = 0; index < channels.size(); ++index) {
    if (channels[index]->name() == channel_name) {
      return true;
    }
  }

  return false;
}

static void BroadcastPart(const std::string& channel_name,
                          const std::string& client_name,
                          std::vector<int>* fd_list) {
  DbContext* db = ContextHolder::GetInstance()->db();
  std::vector<Client*> clients = db->GetClientsByChannelName(channel_name);

  for (size_t index = 0; index < clients.size(); ++index) {
    if (clients[index]->nick_name() == client_name) {
      continue;
    }
    std::string client_fullname = clients[index]->nick_name() + "!" +
                                  clients[index]->user_name() + "@" +
                                  clients[index]->GetHostName();
    std::string response = ":" + client_fullname + " PART " + channel_name;

    clients[index]->SetSendMessage(response);
    fd_list->push_back(clients[index]->GetFd());
  }
}

}  // namespace Just1RCe
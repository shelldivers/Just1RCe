#include "../../includes/commands/joinCommandHandler.h"

#include <string.h>

#include <vector>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

namespace Just1RCe {

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
  Parser parser(message);
  DbContext *db = ContextHolder::GetInstance()->db;
  Client *client = db->GetClient(client_fd);
  std::vector<int> fd_list;

  if (client == nullptr) {
    return fd_list;
  }

  // Get channel names and keys
  std::vector<std::string> channel_names;
  std::vector<std::string> keys;
  if (parser.ParseCommandJoin(&channel_names, &keys) == ERR_NEEDMOREPARAMS) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 461 " +
                           client->nick_name() +
                           " JOIN :Not enough parameters");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  // Join channels
  for (size_t index = 0; index < channel_names.size(); ++index) {
    std::string channel_name = channel_names[index];
    std::string key = "";
    std::string response = ":" + client->nick_name() + " JOIN " + channel_name;
    Channel *channel = db->GetChannel(channel_name);
    std::vector<Client *> clients = db->GetClientsByChannelName(channel_name);

    // Check if the client is already joined
    if (isAlreadyJoined(clients, client_fd)) {
      continue;
    }
    // Get key
    if (keys.size() > index) {
      key = keys[index];
    }
    // Check channel mask
    if (checkChannelMask(channel_name) == ERR_BADCHANMASK) {
      client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 476 " +
                             client->nick_name() + " JOIN :" + channel_name +
                             " :Bad Channel Mask");
      fd_list.push_back(client_fd);

      return fd_list;
    }
    // Create a new channel if it does not exist
    if (channel == nullptr) {
      channel = new Channel(channel_name);
      db->AddChannel(channel);
    }
    // Check channel full
    if (checkChannelFull(*channel, clients.size()) == ERR_CHANNELISFULL) {
      client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 471 " +
                             client->nick_name() + " JOIN :" + channel_name +
                             " :Cannot join channel (+l)");
      fd_list.push_back(client_fd);

      return fd_list;
    }
    // Check channel key
    if (checkChannelKey(*channel, key) == ERR_BADCHANNELKEY) {
      client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 475 " +
                             client->nick_name() + " JOIN :" + channel_name +
                             " :Cannot join channel (+k)");
      fd_list.push_back(client_fd);

      return fd_list;
    }
    // Check invite only
    if (checkInviteOnly(*channel, client_fd) == ERR_INVITEONLYCHAN) {
      client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 473 " +
                             client->nick_name() + " JOIN :" + channel_name +
                             " :Cannot join channel (+i)");
      fd_list.push_back(client_fd);

      return fd_list;
    }
    // Get channel client nicknames for RPL_NAMREPLY
    // and set send join messages to all clients in the channel
    std::string channel_client_nicknames = "";
    for (size_t client_index = 0; client_index < clients.size();
         ++client_index) {
      if (client_index > 0) {
        channel_client_nicknames += " ";
      }
      channel_client_nicknames += clients[client_index]->nick_name();
      clients[client_index]->SetSendMessage(response);
      fd_list.push_back(clients[client_index]->GetFd());
    }

    // Join the client to the channel
    db->JoinClientToChannel(client_fd, channel_name);

    // Set send replies to joined client
    client->SetSendMessage(response);
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " MODE " +
                           channel_name + " " + channel->GetModeAsString());
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 353 " +
                           client->nick_name() + "= " + channel_name + " :" +
                           channel_client_nicknames);
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 366 " +
                           client->nick_name() + " " + channel_name +
                           " :End of /NAMES list.");
    fd_list.push_back(client_fd);
  }

  return fd_list;
}

// - ERR_BADCHANMASK (476)
// :{SERVER_NAME} 476 nick_kenn3 !#$%! :Bad Channel Mask
const int JoinCommandHandler::checkChannelMask(Channel &channel) {
  std::string forbidden_chars = " \x07,:";

  for (size_t index = 0; index < forbidden_chars.size(); ++index) {
    if (channel.name().find(forbidden_chars[index]) != std::string::npos) {
      return ERR_BADCHANMASK;
    }
  }

  return IRC_NOERROR;
}

// - ERR_CHANNELISFULL (471)
// :{SERVER_NAME} 471 nick_ken3 #limited_channel :Cannot join channel (+l)
const int JoinCommandHandler::checkChannelFull(Channel &channel,
                                               const size_t client_num) {
  if (channel.CheckMode('l') && /* channel max num */ <= client_num) {
    return ERR_CHANNELISFULL;
  }

  return IRC_NOERROR;
}

// - ERR_BADCHANNELKEY (475)
// :{SERVER_NAME} 475 nick_ken3 #pass_channel :Cannot join channel (+k)
const int JoinCommandHandler::checkChannelKey(Channel &channel,
                                              const std::string &key) {
  if (channel.CheckMode('k') && channel.key() != key) {
    return ERR_BADCHANNELKEY;
  }

  return IRC_NOERROR;
}

// - ERR_INVITEONLYCHAN (473)
// :{SERVER_NAME} 473 nick_ken3 #invite_only :Cannot join channel (+i)
const int JoinCommandHandler::checkInviteOnly(Channel &channel,
                                              const int client_fd) {
  if (channel.CheckMode('i')) {
    std::vector<Client *> clients = channel.GetInviteList();

    for (size_t index = 0; index < clients.size(); ++index) {
      if (clients[index]->GetFd() == client_fd) {
        return IRC_NOERROR;
      }
    }

    return ERR_INVITEONLYCHAN;
  }

  return IRC_NOERROR;
}

bool JoinCommandHandler::isAlreadyJoined(const std::vector<Client *> &clients,
                                         const int client_fd) {
  for (size_t index = 0; index < clients.size(); ++index) {
    if (clients[index]->GetFd() == client_fd) {
      return true;
    }
  }

  return false;
}

}  // namespace Just1RCe
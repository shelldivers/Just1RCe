
#include <algorithm>
#include <map>
#include <string>
#include <utility>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 *
 * @brief add client to the table
 * @warning caller must check user's nick name is valid before creating client
 * @return return false if the value of user is NULL
 */
bool InMemoryDbContext::AddClient(Client *user) {
  // sanity check
  if (user == NULL) return false;

  // add index table
  client_table_.insert(std::make_pair(user->GetFd(), user));

  return true;
}

/**
 *
 * @brief delete client and it's related data
 */
void InMemoryDbContext::DelClient(int const client_fd) {
  // sanity check
  if (!client_table_.count(client_fd)) return;

  // delete client's modes with 'client_fd'
  DeleteClientModesByClientFd(client_fd);

  // erase all entry with key as client_fd
  client_to_channel_mapping_table_.erase(client_fd);

  // erase all entry with value as client_fd
  while (true) {
    // erase one per loop
    bool isFound = false;
    for (ChannelToClientMappingTableIter itr =
             channel_to_client_mapping_table_.begin();
         itr != channel_to_client_mapping_table_.end(); ++itr) {
      if (itr->second == client_fd) {
        isFound = true;
        channel_to_client_mapping_table_.erase(itr);
        break;
      }
    }
    if (!isFound) break;
  }

  Client *target_client = client_table_[client_fd];

  // delete nick name to fd mapping
  DeleteNickNameToFd(target_client->nick_name());

  // delete all user-host mask in invite list among all channels
  DeleteAllInvitation(
      target_client->user_name() + "@" + target_client->GetHostName(),
      &channel_table_);

  // delete client
  delete target_client;
  client_table_.erase(client_fd);
}

Client *InMemoryDbContext::GetClient(int const client_fd) {
  if (!client_table_.count(client_fd)) return NULL;
  return client_table_[client_fd];
}

void InMemoryDbContext::SetNickNameToFd(std::string const &client_nick_name,
                                        int const fd) {
  nick_name_to_fd_table_[client_nick_name] = fd;
}

int InMemoryDbContext::GetFdByNickName(std::string const &client_nick_name) {
  if (!nick_name_to_fd_table_.count(client_nick_name)) return -1;
  return nick_name_to_fd_table_[client_nick_name];
}

void InMemoryDbContext::DeleteNickNameToFd(
    std::string const &client_nick_name) {
  nick_name_to_fd_table_.erase(client_nick_name);
}

static void DeleteAllInvitation(
    std::string const &user_host_mask,
    std::map<ChannelName, Channel *> *channel_table) {
  // for every channel, erase client's user-host mask
  for (ChannelTableIter itr = channel_table->begin();
       itr != channel_table->end(); ++itr) {
    itr->second->UnInvite(user_host_mask);
  }
}

}  // namespace Just1RCe

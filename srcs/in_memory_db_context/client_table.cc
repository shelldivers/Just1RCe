
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
  for (ChannelToClientMappingTableIter itr =
           channel_to_client_mapping_table_.begin();
       itr != channel_to_client_mapping_table_.end(); ++itr) {
    if (itr->second == client_fd) channel_to_client_mapping_table_.erase(itr);
  }

  // delete client
  delete client_table_[client_fd];
  client_table_.erase(client_fd);
}

Client *InMemoryDbContext::GetClient(int const client_fd) {
  if (!client_table_.count(client_fd)) return NULL;
  return client_table_[client_fd];
}

}  // namespace Just1RCe

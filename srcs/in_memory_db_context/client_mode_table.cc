
#include <string>
#include <utility>

#include "../../includes/client_mode.h"
#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 * @brief set client's mode against channel, use (client's fd + channel name) as
 * key
 * @param client_fd fd of the client, will be part of the key
 * @param channel_name name of the channel, will be part of the key
 * @param mask bit mask that describe it's  mode
 */
void InMemoryDbContext::SetClientMode(int const client_fd,
                                      std::string const &channel_name,
                                      ClientModeMask mask) {
  ModeKey key = std::make_pair(client_fd, channel_name);
  mode_table_[key] = mask;
  return;
}

/**
 * @warning before call, be sure if the client already joint the channel
 * @return if the client is not joined to the channel, return -1
 */
ClientModeMask InMemoryDbContext::GetClientMode(
    int const client_fd, std::string const &channel_name) {
  ModeKey key = std::make_pair(client_fd, channel_name);
  if (mode_table_.find(key) == mode_table_.end()) return -1;
  return mode_table_[key];
}

void InMemoryDbContext::DeleteClientMode(int const client_fd,
                                         std::string const &channel_name) {
  ModeKey key = std::make_pair(client_fd, channel_name);
  ModeTableIter targetIter = mode_table_.find(key);

  // sanity check and erase
  if (targetIter != mode_table_.end()) mode_table_.erase(targetIter);
  return;
}

/**
 *
 * @brief delete all modes with same nick name. using range erase query
 */
void InMemoryDbContext::DeleteClientModesByClientFd(int const client_fd) {
  while (true) {
    bool isFound = false;
    for (ModeTableIter itr = mode_table_.begin(); itr != mode_table_.end();
         ++itr) {
      // erase one mode per loop
      if (itr->first.first == client_fd) {
        isFound = true;
        mode_table_.erase(itr);
        break;
      }
    }
    if (!isFound) break;
  }
}

}  // namespace Just1RCe

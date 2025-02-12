
#include <string>
#include <utility>

#include "../../includes/client_mode.h"
#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

void InMemoryDbContext::SetClientMode(int const client_fd,
                                      std::string const &channel_name,
                                      ClientModeMask mask) {
  ModeKey key = std::make_pair(client_fd, channel_name);
  mode_table_[key] = mask;
  return;
}

ClientModeMask InMemoryDbContext::GetClientMode(
    int const client_fd, std::string const &channel_name) {
  ModeKey key = std::make_pair(client_fd, channel_name);
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


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
  // using lowerbound and upperbound, get range of iterator has 'nick_name'
  ModeTableIter left = mode_table_.lower_bound(
      std::make_pair(client_fd, std::string(1, static_cast<char>(0))));
  ModeTableIter right = mode_table_.upper_bound(
      std::make_pair(client_fd, std::string(1, static_cast<char>(255))));
  for (ModeTableIter itr = left; itr != right; ++itr) mode_table_.erase(itr);
}

}  // namespace Just1RCe

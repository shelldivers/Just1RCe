
#include <utility>

#include "../../includes/client_mode.h"
#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

void InMemoryDbContext::SetClientMode(std::string const &client_nick_name,
                                      std::string const &channel_name,
                                      ClientModeMask mask) {
  ModeKey key = std::make_pair(client_nick_name, channel_name);
  mode_table_[key] = mask;
  return;
}

ClientModeMask InMemoryDbContext::GetClientMode(
    std::string const &client_nick_name, std::string const &channel_name) {
  ModeKey key = std::make_pair(client_nick_name, channel_name);
  return mode_table_[key];
}

void InMemoryDbContext::DeleteClientMode(std::string const &client_nick_name,
                                         std::string const &channel_name) {
  ModeKey key = std::make_pair(client_nick_name, channel_name);
  ModeTableIter targetIter = mode_table_.find(key);

  // sanity check and erase
  if (targetIter != mode_table_.end()) mode_table_.erase(targetIter);
  return;
}

/**
 *
 * @brief delete all modes with same nick name. using range erase query
 */
void InMemoryDbContext::DeleteClientModesByClientName(
    std::string const &client_nick_name) {
  // using lowerbound and upperbound, get range of iterator has 'nick_name'
  ModeTableIter left = mode_table_.lower_bound(
      std::make_pair(client_nick_name, std::string(1, static_cast<char>(0))));
  ModeTableIter right = mode_table_.upper_bound(
      std::make_pair(client_nick_name, std::string(1, static_cast<char>(255))));
  mode_table_.erase(left, right);
}

}  // namespace Just1RCe

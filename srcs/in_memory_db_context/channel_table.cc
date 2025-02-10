
#include <iterator>
#include <utility>
#include <string>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 * @warning check channel name's validity before creating channel
 * @return if room is NULL, return false
 */
bool InMemoryDbContext::AddChannel(Channel *room) {
  if (room == NULL) return false;

  channel_table_.insert(std::make_pair(room->name(), room));

  return true;
}

void InMemoryDbContext::DelChannel(std::string const &channel_name) {
  // sanity check
  if (!channel_table_.count(channel_name)) return;
  // delete channel
  delete channel_table_[channel_name];
  channel_table_.erase(channel_name);
}

Channel *InMemoryDbContext::GetChannel(std::string const &channel_name) {
  if (!channel_table_.count(channel_name)) return NULL;
  return channel_table_[channel_name];
}

size_t InMemoryDbContext::GetNumOfClientInChannel(
    std::string const &channel_name) {
  return channel_to_client_mapping_table_.count(channel_name);
}

}  // namespace Just1RCe

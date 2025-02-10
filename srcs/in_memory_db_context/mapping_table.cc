
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 *
 * @brief add entry in both mapping table and mode table
 */
bool InMemoryDbContext::JoinClientToChannel(std::string const &client_nick_name,
                                            std::string const &channel_name) {
  // add entry in the mapping table
  client_to_channel_mapping_table_.insert(
      std::make_pair(client_nick_name, channel_name));
  channel_to_client_mapping_table_.insert(
      std::make_pair(channel_name, client_nick_name));
  mode_table_.insert(std::make_pair(client_nick_name, channel_name));
}

/**
 *
 * @brief delete entry in both mapping table and mode table
 */
void InMemoryDbContext::PartClientFromChannel(
    std::string const &client_nick_name, std::string const &channel_name) {
  // delete entry in client_to_channel_mapping_table_
  // get half open range that has key value as 'client_nick_name'
  MappingTableIter left =
      client_to_channel_mapping_table_.lower_bound(client_nick_name);
  MappingTableIter right =
      client_to_channel_mapping_table_.upper_bound(client_nick_name);
  // find erase target iterator
  MappingTableIter erase_target_itr =
      std::find(left, right, std::make_pair(client_nick_name, channel_name));
  // if target iterator found, erase
  if (erase_target_itr != right)
    client_to_channel_mapping_table_.erase(erase_target_itr);

  // delete entry in channel_to_client_mapping_table_
  // get half open range that has key value as 'channel_name'
  left = channel_to_client_mapping_table_.lower_bound(channel_name);
  right = channel_to_client_mapping_table_.upper_bound(channel_name);
  // find erase target iterator
  erase_target_itr =
      std::find(left, right, std::make_pair(channel_name, client_nick_name));
  // if target iterator found, erase
  if (erase_target_itr != right)
    channel_to_client_mapping_table_.erase(erase_target_itr);

  // erase client mode
  mode_table_.erase(std::make_pair(client_nick_name, channel_name));
}

std::vector<Channel *> InMemoryDbContext::GetChannelsByClientNickName(
    std::string const &client_nick_name) {
  std::vector<Channel *> channels;

  // get range
  MappingTableIter left =
      client_to_channel_mapping_table_.lower_bound(client_nick_name);
  MappingTableIter right =
      client_to_channel_mapping_table_.upper_bound(client_nick_name);

  // transform channel name too channel pointers
  for (MappingTableIter itr = left; itr != right; ++itr) {
    channels.push_back(channel_table_[itr->second]);
  }
  return channels;
}

std::vector<Client *> InMemoryDbContext::GetClientsByChannelName(
    std::string const &channel_name) {
  std::vector<Client *> clients;

  // get range
  MappingTableIter left =
      channel_to_client_mapping_table_.lower_bound(channel_name);
  MappingTableIter right =
      channel_to_client_mapping_table_.upper_bound(channel_name);

  // transform channel name too channel pointers
  for (MappingTableIter itr = left; itr != right; ++itr) {
    clients.push_back(client_table_[itr->second]);
  }
  return clients;
}

}  // namespace Just1RCe

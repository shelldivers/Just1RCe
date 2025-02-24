
#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 *
 * @brief add entry in both mapping table and mode table
 * @return if the user count is full, return fail
 */
bool InMemoryDbContext::JoinClientToChannel(int const client_fd,
                                            std::string const &channel_name) {
  // user number check join availability
  Channel *target_channel = GetChannel(channel_name);
  if (target_channel->cur_user_count() == target_channel->max_user_num()) {
    return false;
  }

  // add entry in the mapping table
  client_to_channel_mapping_table_.insert(
      std::make_pair(client_fd, channel_name));
  channel_to_client_mapping_table_.insert(
      std::make_pair(channel_name, client_fd));

  ModeKey key = std::make_pair(client_fd, channel_name);
  mode_table_.insert(std::make_pair(key, 0));

  // get channel and increase count
  target_channel->IncreaseUserCount();

  return true;
}

/**
 *
 * @brief delete entry in both mapping table and mode table
 */
void InMemoryDbContext::PartClientFromChannel(int const client_fd,
                                              std::string const &channel_name) {
  // delete entry in client_to_channel_mapping_table_
  {
    // get half open range that has key as 'client_fd'
    ClientToChannelMappingTableIter left =
        client_to_channel_mapping_table_.lower_bound(client_fd);
    ClientToChannelMappingTableIter right =
        client_to_channel_mapping_table_.upper_bound(client_fd);
    // find target iter to erase
    ClientToChannelMappingTableIter erase_target_itr =
        std::find(left, right,
                  std::pair<const int, std::string>(client_fd, channel_name));
    // if target iterator found, erase
    if (erase_target_itr != right)
      client_to_channel_mapping_table_.erase(erase_target_itr);
  }

  // delete entry in channel_to_client_mapping_table_
  {
    // get half open range that has key as 'channel_name'
    ChannelToClientMappingTableIter left =
        channel_to_client_mapping_table_.lower_bound(channel_name);
    ChannelToClientMappingTableIter right =
        channel_to_client_mapping_table_.upper_bound(channel_name);
    // find target iterator to erase
    ChannelToClientMappingTableIter erase_target_itr =
        std::find(left, right,
                  std::pair<const std::string, int>(channel_name, client_fd));
    // if target iterator found, erase
    if (erase_target_itr != right)
      channel_to_client_mapping_table_.erase(erase_target_itr);
  }

  // erase client mode
  DeleteClientMode(client_fd, channel_name);

  // user count
  Channel *target_channel = GetChannel(channel_name);
  target_channel->DecreaseUserCount();

  // if client count == 0, delete channel
  if (target_channel->cur_user_count() == 0) {
    DelChannel(channel_name);
  }
}

std::vector<Channel *> InMemoryDbContext::GetChannelsByClientFd(
    int const client_fd) {
  std::vector<Channel *> channels;

  // get range
  ClientToChannelMappingTableIter left =
      client_to_channel_mapping_table_.lower_bound(client_fd);
  ClientToChannelMappingTableIter right =
      client_to_channel_mapping_table_.upper_bound(client_fd);

  // transform channel name too channel pointers
  for (ClientToChannelMappingTableIter itr = left; itr != right; ++itr) {
    channels.push_back(channel_table_[itr->second]);
  }
  return channels;
}

std::vector<Client *> InMemoryDbContext::GetClientsByChannelName(
    std::string const &channel_name) {
  std::vector<Client *> clients;

  // get range
  ChannelToClientMappingTableIter left =
      channel_to_client_mapping_table_.lower_bound(channel_name);
  ChannelToClientMappingTableIter right =
      channel_to_client_mapping_table_.upper_bound(channel_name);

  // transform channel name too channel pointers
  for (ChannelToClientMappingTableIter itr = left; itr != right; ++itr) {
    clients.push_back(client_table_[itr->second]);
  }
  return clients;
}

}  // namespace Just1RCe


#include <string>

#include "../../includes/channel.h"

namespace Just1RCe {

/**
 * @brief invite a client to the channel
 * @param user_host_mask user_name + '@' + host_name
 */
bool Channel::Invite(std::string const &user_host_mask) {
  if (invite_list_.find(user_host_mask) != invite_list_.end()) return false;
  invite_list_.insert(user_host_mask);
  return true;
}

/**
 * @brief un-invite a client from the channel
 * @param user_host_mask user_name + '@' + host_name
 */
bool Channel::UnInvite(std::string const &user_host_mask) {
  if (invite_list_.find(user_host_mask) == invite_list_.end()) return false;
  invite_list_.erase(user_host_mask);
  return true;
}

/**
 * @brief check if the client is invited to the channel
 * @param user_host_mask user_name + '@' + host_name
 */
bool Channel::IsInvited(std::string const &user_host_mask) {
  return invite_list_.find(user_host_mask) != invite_list_.end();
}

}  // namespace Just1RCe

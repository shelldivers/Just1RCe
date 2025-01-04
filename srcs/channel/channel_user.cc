
#include <set>
#include <string>

#include "../includes/channel.h"

namespace Just1RCe {

void Channel::AddUser(std::string const &target_user_name) {
  if (CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT) &&
      user_nicknames_.size() >= max_user_num_)
    return;
  user_nicknames_.insert(target_user_name);
}

void Channel::DeleteUser(std::string const &target_user_name) {
  if (user_nicknames_.find(target_user_name) != user_nicknames_.end())
    user_nicknames_.erase(target_user_name);
}

bool Channel::CheckUser(std::string const &target_user_name) const {
  return (user_nicknames_.find(target_user_name) != user_nicknames_.end());
}
}  // namespace Just1RCe

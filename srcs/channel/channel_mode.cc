
#include <stdexcept>
#include <string>

#include "../includes/channel.h"

namespace Just1RCe {

ChannelModeMask Channel::GetMode() const { return mod_; }

/**
 * @brief get mode of the channel in formatted string. This method is for MODE
 * @return formatted string of channel's mode. ex) "+intk"
 */
std::string Channel::GetModeAsString() const {
  if (mod_ == JUST1RCE_SRCS_CHANNEL_MOD_NONE) return "";
  std::string result("+");
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY) result.push_back('i');
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_MSG_OPRT_ONLY) result.push_back('m');
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_MSG_IN_ONLY) result.push_back('n');
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_TOPIC_OPRT_ONLY) result.push_back('t');
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET) result.push_back('k');
  if (mod_ & JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT) result.push_back('l');
  return result;
}

void Channel::SetMode(ChannelModeMask const flags) { mod_ |= flags; }
void Channel::UnsetMode(ChannelModeMask const flags) { mod_ &= ~flags; };

/**
 * @brief compare a mask
 * @param flags mask to compare
 * @return if flags and mod_ has common, return true.
 */
bool Channel::CheckMode(ChannelModeMask const flags) const {
  return (mod_ & flags);
};
}  // namespace Just1RCe

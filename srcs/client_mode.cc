
#include "client.h"

namespace Just1RCe {

/**
 * @return formatted mode of client
 */
std::string Client::GetMode() const {
  std::string result("+");
  if (mode_mask_ & JUST1RCE_SRCS_CLIENT_MOD_INVISIBLE) result.push_back('i');
  if (mode_mask_ & JUST1RCE_SRCS_CLIENT_MOD_WALLOPS) result.push_back('w');
  if (mode_mask_ & JUST1RCE_SRCS_CLIENT_MOD_OPERATOR) result.push_back('o');
  if (mode_mask_ & JUST1RCE_SRCS_CLIENT_MOD_REGISTERED) result.push_back('r');
  if (mode_mask_ & JUST1RCE_SRCS_CLIENT_MOD_RECEIVE_SERVER)
    result.push_back('s');

  return result;
}

/**
 * @param flags bitset of flags to set
 */
void Client::SetMode(uint const flags) { mode_mask_ |= flags; }

/**
 * @param flags bitset of flags to unset
 */
void Client::UnsetMode(uint const flags) { mode_mask_ &= ~flags; }

/**
 * @param flags bitset of flags to compare
 * @return true if all the mask in the flags are setted.
 */
bool Client::CheckMode(uint const flags) const {
  return (flags & mode_mask_) == flags;
}

}  // namespace Just1RCe

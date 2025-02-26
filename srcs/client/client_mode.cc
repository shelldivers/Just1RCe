
#include "../../includes/client_mode.h"

#include <string>

namespace Just1RCe {

/**
 * @brief stringify current mode of the client
 * @param target target mask to be converted
 * @return formatted mode of client
 * @throws none
 */
std::string ConvertModeToString(ClientModeMask const target) {
  std::string result("+");
  if (target & JUST1RCE_SRCS_CLIENT_MOD_INVISIBLE) result.push_back('i');
  if (target & JUST1RCE_SRCS_CLIENT_MOD_WALLOPS) result.push_back('w');
  if (target & JUST1RCE_SRCS_CLIENT_MOD_OPERATOR) result.push_back('o');
  if (target & JUST1RCE_SRCS_CLIENT_MOD_REGISTERED) result.push_back('r');
  if (target & JUST1RCE_SRCS_CLIENT_MOD_RECEIVE_SERVER) result.push_back('s');

  return result;
}

/**
 * @brief set bitset of flags to the client's mode
 * @param target target mask to add flags
 * @param flags bitset of flags to set
 * @return none
 * @throws none
 */
void AddFlagsToMode(ClientModeMask *target, ClientModeMask const flags) {
  *target |= flags;
}

/**
 * @brief unset bitset of flags to the client's mode
 * @param target target mask to sub flags
 * @param flags bitset of flags to unset
 * @return none
 * @throws none
 */
void SubFlagsFromMode(ClientModeMask *target, ClientModeMask const flags) {
  *target &= ~flags;
}

/**
 * @brief chekc currently setted mode of the client
 * @param target mode to compare with
 * @param flags bitset of flags to compare
 * @return true if all the mask in the flags are setted.
 * @throws none
 */
bool CheckMode(ClientModeMask const target, ClientModeMask const flags) {
  return (flags & target) == flags;
}

}  // namespace Just1RCe

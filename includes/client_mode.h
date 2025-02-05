
#ifndef JUST1RCE_SRCS_CLIENT_MODE_H
#define JUST1RCE_SRCS_CLIENT_MODE_H

#include <string>

namespace Just1RCe {

typedef unsigned int ClientModeMask;

// Mod flags
// describes client's permission, bitmasking
#define JUST1RCE_SRCS_CLIENT_MOD_INVISIBLE 0b000001
#define JUST1RCE_SRCS_CLIENT_MOD_WALLOPS 0b000010
#define JUST1RCE_SRCS_CLIENT_MOD_OPERATOR 0b000100
#define JUST1RCE_SRCS_CLIENT_MOD_REGISTERED 0b001000
#define JUST1RCE_SRCS_CLIENT_MOD_RECEIVE_SERVER 0b010000

std::string ConvertModeToString(ClientModeMask const target);
inline void AddFlagsToMode(ClientModeMask *target, ClientModeMask const flags);
inline void SubFlagsFromMode(ClientModeMask *target,
                             ClientModeMask const flags);
inline bool CheckMode(ClientModeMask const target,
                      ClientModeMask const to_find);

}  // namespace Just1RCe

#endif

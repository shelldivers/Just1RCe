
#include <sstream>
#include <string>
#include <vector>

#include "../../../includes/channel.h"
#include "../../../includes/client.h"
#include "../../../includes/command_handler.h"
#include "../../../includes/commands/mode_command_handler.h"
#include "../../../includes/context_holder.h"
#include "../../../includes/dbcontext.h"

namespace Just1RCe {

void ModeCommandHandler::ProcessModeI(Client *const target_client,
                                      Channel *const target_channel,
                                      std::vector<int> *fd_list,
                                      char const oprt) {
  if (oprt == '+') {
    // add
    target_channel->SetMode(JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY);
  } else {
    // erase
    target_channel->UnsetMode(JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY);
  }
  BroadCastMode(fd_list, target_channel, target_client, oprt, 'i', "");
}

}  // namespace Just1RCe

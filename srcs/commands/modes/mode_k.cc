
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

void ModeCommandHandler::ProcessModeK(Client *const target_client,
                                      Channel *const target_channel,
                                      std::vector<int> *fd_list,
                                      char const oprt,
                                      std::stringstream *arguments) {
  if (oprt == '+') {
    std::string new_key;
    *arguments >> new_key;

    if (new_key.empty() ||
        target_channel->CheckMode(JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET)) {
      // no argument or already key set, skip
      return;
    }

    target_channel->SetMode(JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET);
    target_channel->set_key(new_key);
    BroadCastMode(fd_list, target_channel, target_client, oprt, 'k', new_key);
  } else {
    target_channel->UnsetMode(JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET);
    BroadCastMode(fd_list, target_channel, target_client, oprt, 'k', "");
  }
}

}  // namespace Just1RCe

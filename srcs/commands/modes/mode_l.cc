
#include <cstdlib>
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

static bool CheckAllDigit(std::string const &str) {
  for (size_t i = 0; i < str.size(); ++i) {
    if (!std::isdigit(str[i])) return false;
  }
  return true;
}

void ModeCommandHandler::ProcessModeL(Client *const target_client,
                                      Channel *const target_channel,
                                      std::vector<int> *fd_list,
                                      char const oprt,
                                      std::stringstream *arguments) {
  if (oprt == '+') {
    std::string arg;
    *arguments >> arg;
    int const new_limit = std::atoi(arg.c_str());

    if (arg.empty() || !CheckAllDigit(arg) || new_limit < 1) {
      // no argument or already key set, skip
      return;
    }

    target_channel->SetMode(JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT);
    target_channel->set_max_user_num(new_limit);
    BroadCastMode(fd_list, target_channel, target_client, oprt, 'l', arg);
  } else {
    target_channel->UnsetMode(JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT);
    BroadCastMode(fd_list, target_channel, target_client, oprt, 'l', "");
  }
}
}  // namespace Just1RCe

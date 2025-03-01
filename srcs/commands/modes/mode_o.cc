
#include <sstream>
#include <string>
#include <vector>

#include "../../../includes/channel.h"
#include "../../../includes/client.h"
#include "../../../includes/client_mode.h"
#include "../../../includes/command_handler.h"
#include "../../../includes/commands/mode_command_handler.h"
#include "../../../includes/context_holder.h"
#include "../../../includes/dbcontext.h"

namespace Just1RCe {

void ModeCommandHandler::ProcessModeO(Client *const target_client,
                                      Channel *const target_channel,
                                      std::vector<int> *fd_list,
                                      char const oprt,
                                      std::stringstream *arguments) {
  DbContext *db = ContextHolder::GetInstance()->db();
  std::string arg;

  if (!std::getline(*arguments, arg, ',') || arg.empty()) {
    return;
  }
  if (oprt == '+') {
    Client *next_oprt_client = db->GetClient(db->GetFdByNickName(arg));
    if (next_oprt_client == NULL) {
      // next operator client not found
      return;
    }

    // move operator
    // disable current operator
    ClientModeMask prev_oprt_mode =
        db->GetClientMode(target_client->GetFd(), target_channel->name());
    SubFlagsFromMode(&prev_oprt_mode, JUST1RCE_SRCS_CLIENT_MOD_OPERATOR);
    db->SetClientMode(target_client->GetFd(), target_channel->name(),
                      prev_oprt_mode);

    // apply to next operator
    ClientModeMask next_oprt_mode =
        db->GetClientMode(next_oprt_client->GetFd(), target_channel->name());
    AddFlagsToMode(&next_oprt_mode, JUST1RCE_SRCS_CLIENT_MOD_OPERATOR);
    db->SetClientMode(next_oprt_client->GetFd(), target_channel->name(),
                      next_oprt_mode);

    target_channel->operator_fd(next_oprt_client->GetFd());

    // Broad cast
    BroadCastMode(fd_list, target_channel, target_client, oprt, 'o', arg);
  }
  /*
    else {
       // remove operator
       // there is one operator per channel, we cannot erase
       // operator, ignore
    }
  */
}
}  // namespace Just1RCe

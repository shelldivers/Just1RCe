#include "../../includes/command_mapping.h"

#include <map>
#include <string>

#include "../../includes/command_handler.h"
#include "../../includes/commands/invite_command_handler.h"
#include "../../includes/commands/join_command_handler.h"
#include "../../includes/commands/kick_command_handler.h"
#include "../../includes/commands/mode_command_handler.h"
#include "../../includes/commands/nick_command_handler.h"
#include "../../includes/commands/part_command_handler.h"
#include "../../includes/commands/pass_command_handler.h"
#include "../../includes/commands/ping_command_handler.h"
#include "../../includes/commands/privmsg_command_handler.h"
#include "../../includes/commands/quit_command_handler.h"
#include "../../includes/commands/topic_command_handler.h"
#include "../../includes/commands/user_command_handler.h"

namespace Just1RCe {

void RegistCommandHandlers(CommandMapping* cmd_map,
                           const std::string& password) {
  (*cmd_map)["PASS"] = new PassCommandHandler(password);
  (*cmd_map)["NICK"] = new NickCommandHandler();
  (*cmd_map)["USER"] = new UserCommandHandler();
  (*cmd_map)["PING"] = new PingCommandHandler();
  (*cmd_map)["JOIN"] = new JoinCommandHandler();
  (*cmd_map)["QUIT"] = new QuitCommandHandler();
  (*cmd_map)["PART"] = new PartCommandHandler();
  (*cmd_map)["INVITE"] = new InviteCommandHandler();
  (*cmd_map)["TOPIC"] = new TopicCommandHandler();
  (*cmd_map)["KICK"] = new KickCommandHandler();
  (*cmd_map)["MODE"] = new ModeCommandHandler();
  (*cmd_map)["PRIVMSG"] = new PrivmsgCommandHandler();
}

}  // namespace Just1RCe

#include "../../includes/command_mapping.h"

#include <map>
#include <string>

#include "../../includes/command_handler.h"

namespace Just1RCe {

void RegistCommandHandlers(CommandMapping *cmd_map, const std::string& password) {
	(*cmd_map)["PASS"] = new PassCommandHandler(password);
	(*cmd_map)["NICK"] = new NickCommandHandler();
	(*cmd_map)["USER"] = new UserCommandHandler();
}

}
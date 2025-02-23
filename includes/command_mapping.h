#ifndef JUST1RCE_INCLUDES_COMMAND_MAPPING_H_
#define JUST1RCE_INCLUDES_COMMAND_MAPPING_H_

#include <map>
#include <string>

class CommandHandler;

typedef std::map<std::string, CommandHandler *> CommandMapping;

namespace just1RCe {

void RegistCommandHandlers(CommandMapping *cmd_map, const std::string& password);

}

#endif  // JUST1RCE_INCLUDES_COMMAND_MAPPING_H_
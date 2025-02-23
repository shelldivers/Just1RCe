#ifndef JUST1RCE_INCLUDES_COMMAND_MAPPING_H_
#define JUST1RCE_INCLUDES_COMMAND_MAPPING_H_

#include <map>
#include <string>

class CommandHandler;

namespace Just1RCe {

typedef std::map<std::string, CommandHandler *> CommandMapping;

void RegistCommandHandlers(CommandMapping *cmd_map,
                           const std::string &password);

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMAND_MAPPING_H_

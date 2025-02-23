#ifndef JUST1RCE_INCLUDES_COMMAND_MAPPING_H_
#define JUST1RCE_INCLUDES_COMMAND_MAPPING_H_

#include <map>
#include <string>

class CommandHandler;

typedef std::map<std::string, Just1RCe::CommandHandler *> CommandMapping;

namespace Just1RCe {

void RegistCommandHandlers(CommandMapping *cmd_map,
                           const std::string &password);

}  // namespace Just1RCe

#endif  // JUST1RCE_INCLUDES_COMMAND_MAPPING_H_


#ifndef JUST1RCE_INCLUDES_COMMANDS_MODE_COMMAND_HANDLER_H
#define JUST1RCE_INCLUDES_COMMANDS_MODE_COMMAND_HANDLER_H

#include <sstream>
#include <string>
#include <vector>

#include "../channel.h"
#include "../client.h"
#include "../command_handler.h"

namespace Just1RCe {

class ModeCommandHandler : public CommandHandler {
 private:
  ModeCommandHandler(ModeCommandHandler const &);
  ModeCommandHandler &operator=(ModeCommandHandler const &);

  // common utility
  void BroadCastMode(std::vector<int> *fd_list, Channel *const target_channel,
                     Client *const target_client, char const mode_oprt,
                     char const mode_identifier, std::string const &mode_arg);

  void ModifyModes(Client *const target_client, Channel *const target_channel,
                   std::string const &mode_string, std::string const &mode_args,
                   std::vector<int> *fd_list);
  // mode to handle
  void ProcessModeI(Client *const target_client, Channel *const target_channel,
                    std::vector<int> *fd_list, char const oprt);
  void ProcessModeT(Client *const target_client, Channel *const target_channel,
                    std::vector<int> *fd_list, char const oprt);
  void ProcessModeK(Client *const target_client, Channel *const target_channel,
                    std::vector<int> *fd_list, char const oprt,
                    std::stringstream *arguments);
  void ProcessModeO(Client *const target_client, Channel *const target_channel,
                    std::vector<int> *fd_list, char const oprt,
                    std::stringstream *arguments);
  void ProcessModeL(Client *const target_client, Channel *const target_channel,
                    std::vector<int> *fd_list, char const oprt,
                    std::stringstream *arguments);

 public:
  ModeCommandHandler();
  ~ModeCommandHandler();
  std::vector<int> operator()(const int client_fd, const std::string &message);
};

}  // namespace Just1RCe

#endif

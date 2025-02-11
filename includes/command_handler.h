
#ifndef JUST1RCE_SRCS_COMMAND_HANDLER_H
#define JUST1RCE_SRCS_COMMAND_HANDLER_H

#include <string>
#include <vector>

namespace Just1RCe {

/**
 *
 * @brief interface for command handler, will be mappend with command string
 * @date 2025-02-09
 * @author eldeshue
 *
 */
class CommandHandler {
 public:
  virtual ~CommandHandler() {}

  /**
   * @brief handle command
   * @param client_fd fd to identify client that te message originate from
   * @param message raw text message from client, will be parsed, no trailing
   * cr-lf
   * @return vector of int(fd), to register write event
   */
  virtual std::vector<int> operator()(int const client_fd,
                                      std::string const &meesage) = 0;
};

}  // namespace Just1RCe

#endif

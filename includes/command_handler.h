
#ifndef JUST1RCE_SRCS_COMMAND_HANDLER_H
#define JUST1RCE_SRCS_COMMAND_HANDLER_H

#include <string>

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
  ~CommandHandler() {}

  virtual void operator()(std::string const &nick_name,
                          std::string const &meesage) = 0;
};

}  // namespace Just1RCe

#endif

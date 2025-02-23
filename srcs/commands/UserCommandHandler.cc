#include "../../includes/commands/userCommandHandler.h"

#include <string>
#include <vector>

#include "../../includes/client.h"
#include "../../includes/commands/commandHandler.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parse/parser.h"
#include "../../includes/response_generator.h"

namespace Just1RCe {

UserCommandHandler::UserCommandHandler() {}

UserCommandHandler::~UserCommandHandler() {}

/**
 * @brief USER command handler
 *
 * @param client_fd fd to identify client that te message originate from
 * @param message raw text message from client
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * Get 2 types of Numeric
 * - ERR_NEEDMOREPARAMS : not enough parameters
 * - ERR_ALREADYREGISTRED : already registered
 */
std::vector<int> UserCommandHandler::operator()(const int client_fd,
                                                const std::string& message) {
  Client* client = ContextHolder::GetInstance()->db()->GetClient(client_fd);

  if (client == NULL || client->IsPassed() == false) {
    return std::vector<int>();
  }

  std::string username;
  std::string realname;
  Just1RCe::p parser(message);

  // Set username and realname
  // No message is sent to the client
  client->set_user_name(username);
  client->set_real_name(realname);

  return fd_list;
}

const int GetUserErrorCode(const Client& client, std::string username,
                           std::string realname) {
  if (username.empty() == true || realname.empty() == true) {
    return ERR_NEEDMOREPARAMS;
  }
  if (client.user_name().empty() == false || client.real_name().empty() == false) {
    return ERR_ALREADYREGISTRED;
  }

}  // namespace Just1RCe

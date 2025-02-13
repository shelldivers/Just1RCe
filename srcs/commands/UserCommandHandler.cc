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
std::vector<int> UserCommandHandler::operator()(const int client_fd, const std::string &message) {
  Parser parser(message);
  DbContext* db = ContextHolder::GetInstance()->db;
  Client* client = db->GetClient(client_fd);
  std::vector<int> fd_list;

  if (client == nullptr) {
    return fd_list;
  }

  if (client->user_name().empty() == false) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 462 " +
                           client->nick_name() + " :You may not reregister");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  std::string username, realname;
  int numeric = parser.ParseCommandUser(&username, &realname);

  if (numeric) {
    client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 461 " +
                           client->nick_name() + " :Not enough parameters");
    fd_list.push_back(client_fd);

    return fd_list;
  }

  client->set_user_name(username);
  client->set_real_name(realname);
  fd_list.push_back(client_fd);

  return fd_list;
}

}  // namespace Just1RCe

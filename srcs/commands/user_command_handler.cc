#include "../../includes/commands/user_command_handler.h"

#include <string>
#include <vector>

#include "../../includes/client.h"
#include "../../includes/command_handler.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"
#include "../../includes/response_generator.h"

namespace Just1RCe {

UserCommandHandler::UserCommandHandler() {}

UserCommandHandler::~UserCommandHandler() {}

/**
 * @brief USER command handler
 *
 * @param client_fd fd to identify client that the message originate from
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

  // Parse
  std::string username;
  std::string realname;
  Parser parser(message);
  parser.ParseCommandUser(&username, &realname);

  // Get numeric
  int numeric = CheckUser(*client, username, realname);
  if (numeric != IRC_NOERROR) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        numeric,
        ResponseArguments(numeric, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Set user name and real name
  client->set_user_name(username);
  client->set_real_name(realname);

  // If the client is authenticated, send welcome message
  if (client->IsAuthenticated() == true) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        RPL_WELCOME,
        ResponseArguments(RPL_WELCOME, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  return std::vector<int>();
}

int UserCommandHandler::CheckUser(const Client& client, std::string username,
                                  std::string realname) {
  if (username.empty() == true || realname.empty() == true) {
    return ERR_NEEDMOREPARAMS;
  }

  if (client.user_name().empty() == false ||
      client.real_name().empty() == false) {
    return ERR_ALREADYREGISTERED;
  }

  return IRC_NOERROR;
}

}  // namespace Just1RCe

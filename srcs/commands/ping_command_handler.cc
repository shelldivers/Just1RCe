#include "../../includes/commands/ping_command_handler.h"

#include <string.h>

#include <vector>

#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"
#include "../../includes/response_generator.h"

namespace Just1RCe {

PingCommandHandler::PingCommandHandler() {}

PingCommandHandler::~PingCommandHandler() {}

/**
 * @brief PING command handler
 *
 * @param client_fd fd to identify client that te message originate from
 * @param message raw text message from client, will be parsed
 *
 * @return vector of int(fd), to register write event
 *
 * @details
 * Get 2 types of Numeric
 * - ERR_NOORIGIN (409) : No origin specified
 * - ERR_NOTREGISTERED (451) : You have not registered
 */
std::vector<int> PingCommandHandler::operator()(const int client_fd,
                                                const std::string& message) {
  Parser parser(message);
  Client* client = ContextHolder::GetInstance()->db()->GetClient(client_fd);

  if (client == NULL || client->IsAuthenticated() == false) {
    return std::vector<int>();
  }

  // Get token
  std::string token;
  parser.ParseCommandPing(&token);

  int numeric = CheckPing(*client, token);
  if (numeric != IRC_NOERROR) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        numeric,
        ResponseArguments(numeric, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Send PONG message
  std::string response = "PONG :" + token;
  client->SetSendMessage(response);

  return std::vector<int>(1, client_fd);
}

const int PingCommandHandler::CheckPing(const Client& client,
                                        const std::string& token) {
  if (token.empty() == true) {
    return ERR_NOORIGIN;
  }

  return IRC_NOERROR;
}

}  // namespace Just1RCe

#include "../../includes/commands/pass_command_handler.h"

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

PassCommandHandler::PassCommandHandler(const std::string& password)
    : password_(password) {}

PassCommandHandler::~PassCommandHandler() {}

/**
 * @brief Pass command handler
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
std::vector<int> PassCommandHandler::operator()(const int client_fd,
                                                const std::string& message) {
  Client* client = ContextHolder::GetInstance()->db()->GetClient(client_fd);

  if (client == NULL) {
    return std::vector<int>();
  }

  // Parse
  std::string password;
  Parser parser(message);
  parser.ParseCommandPass(&password);
  std::vector<std::string> token_stream = parser.GetTokenStream();

  if (token_stream.size() < 2) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        ERR_NEEDMOREPARAMS,
        ResponseArguments(ERR_NEEDMOREPARAMS, *client, NULL, token_stream));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Get numeric
  int numeric = CheckPass(*client, password);
  if (numeric != IRC_NOERROR) {
    ResponseGenerator& generator = ResponseGenerator::GetInstance();
    std::string response = generator.GenerateResponse(
        numeric,
        ResponseArguments(numeric, *client, NULL, parser.GetTokenStream()));

    client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // Set client as passed
  client->PassDone();

  return std::vector<int>();
}

int PassCommandHandler::CheckPass(const Client& client, std::string password) {
  if (client.IsPassed() == true) {
    return ERR_ALREADYREGISTERED;
  }

  if (password_ != password) {
    return ERR_PASSWDMISMATCH;
  }

  return IRC_NOERROR;
}

}  // namespace Just1RCe

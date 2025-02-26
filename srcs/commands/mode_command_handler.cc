
#include "../../includes/commands/mode_command_handler.h"

#include <sstream>
#include <string>
#include <vector>

#include "../../includes/channel.h"
#include "../../includes/client.h"
#include "../../includes/command_handler.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"
#include "../../includes/response_generator.h"

namespace Just1RCe {

ModeCommandHandler::ModeCommandHandler() {}
ModeCommandHandler::~ModeCommandHandler() {}

void ModeCommandHandler::BroadCastMode(std::vector<int> *fd_list,
                                       Channel *const target_channel,
                                       Client *const target_client,
                                       char const mode_oprt,
                                       char const mode_identifier,
                                       std::string const &mode_arg) {
  std::vector<Client *> clients =
      ContextHolder::GetInstance()->db()->GetClientsByChannelName(
          target_channel->name());

  std::stringstream message;
  message << ':' << target_client->nick_name() << '!'
          << target_client->user_name() << "@" << target_client->GetHostName()
          << ".IP MODE " << target_channel->name() << ' ' << mode_oprt
          << mode_identifier << ' ' << mode_arg;

  for (size_t i = 0; i < clients.size(); ++i) {
    clients[i]->SetSendMessage(message.str());
    fd_list->push_back(clients[i]->GetFd());
  }
}

static bool CheckModeStringFormat(std::string const &mode_string);

std::vector<int> ModeCommandHandler::operator()(const int client_fd,
                                                const std::string &message) {
  // parse message and get resources
  std::string target_channel_name, mode_string, mode_arguments;
  Parser parser(message);
  parser.ParseCommandMode(&target_channel_name, &mode_string, &mode_arguments);

  Client *const target_client =
      ContextHolder::GetInstance()->db()->GetClient(client_fd);
  Channel *const target_channel =
      ContextHolder::GetInstance()->db()->GetChannel(target_channel_name);

  // target channel not found
  if (target_channel == NULL) {
    std::string response = ResponseGenerator::GetInstance().GenerateResponse(
        ERR_NOSUCHCHANNEL,
        ResponseArguments(ERR_NOSUCHCHANNEL, *target_client, target_channel,
                          parser.GetTokenStream()));
    target_client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // check input parameter
  if (CheckModeStringFormat(mode_string)) {
    std::string response = ResponseGenerator::GetInstance().GenerateResponse(
        RPL_CHANNELMODEIS,
        ResponseArguments(RPL_CHANNELMODEIS, *target_client, target_channel,
                          parser.GetTokenStream()));
    target_client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // check client authority
  if (target_channel->GetOperatorFd() != client_fd) {
    std::string response = ResponseGenerator::GetInstance().GenerateResponse(
        ERR_CHANOPRIVSNEEDED,
        ResponseArguments(ERR_CHANOPRIVSNEEDED, *target_client, target_channel,
                          parser.GetTokenStream()));
    target_client->SetSendMessage(response);
    return std::vector<int>(1, client_fd);
  }

  // do MODE command
  std::vector<int> fd_list;
  ModifyModes(target_client, target_channel, mode_string, mode_arguments,
              &fd_list);
  return fd_list;
}

static bool CheckModeIdentifier(char const c) {
  return ((c == 'i') || (c == 't') || (c == 'k') || (c == 'o') || (c == 'l'));
}

static bool CheckModeOperator(char const c) {
  return ((c == '+') || (c == '-'));
}

static bool CheckModeStringFormat(std::string const &mode_string) {
  if (mode_string.empty() || !CheckModeOperator(mode_string[0])) {
    return false;
  }

  for (size_t i = 1; i < mode_string.size(); ++i) {
    if (!CheckModeIdentifier(mode_string[i])) return false;
  }
  return true;
}

void ModeCommandHandler::ModifyModes(Client *const target_client,
                                     Channel *const target_channel,
                                     std::string const &mode_string,
                                     std::string const &mode_args,
                                     std::vector<int> *fd_list) {
  std::stringstream ss(mode_args);
  char oprt = '\0';

  for (int i = 0; i < mode_string.size(); ++i) {
    if (CheckModeOperator(mode_string[i])) {
      // change method
      oprt = mode_string[i];
    } else {
      if (mode_string[i] == 'i') {
        ProcessModeI(target_client, target_channel, fd_list, oprt);
      } else if (mode_string[i] == 't') {
        ProcessModeT(target_client, target_channel, fd_list, oprt);
      } else if (mode_string[i] == 'k') {
        ProcessModeK(target_client, target_channel, fd_list, oprt, &ss);
      } else if (mode_string[i] == 'o') {
        ProcessModeO(target_client, target_channel, fd_list, oprt, &ss);
      } else if (mode_string[i] == 'l') {
        ProcessModeL(target_client, target_channel, fd_list, oprt, &ss);
      } else {
        // unreachable, unknown mode, skip
      }
    }
  }
}

}  // namespace Just1RCe

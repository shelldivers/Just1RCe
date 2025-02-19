#include "../../response_generator.h"

#include <string>
#include <vector>

#include "../../config.h"
#include "../../includes/Client.h"
#include "../../includes/channel.h"
#include "../../includes/numeric.h"

namespace Just1RCe {

const char* ResponseGenerator::response_templates_[] = {
    ":%s 001 %c :Welcome to the SHELLDIVERS Network, %n",
    ":%s 002 %c :Your host is %s, running version %v",
    ":%s 324 %c %h %m",
    ":%s 331 %c %h :No topic is set",
    ":%s 332 %c %h :%t",
    ":%s 341 %c %n %h",
    ":%s 353 %c = %h :%n{ %n}",
    ":%s 366 %c %h :End of /NAMES list",
    ":%s 401 %c <nickname> :No such nick/channel",
    ":%s 403 %c %h :No such channel",
    ":%s 404 %c %h :Cannot send to channel",
    ":%s 405 %c %h :You have joined too many channels",
    ":%s 409 %c :No origin specified",
    ":%s 411 %c :No recipient given (%d)",
    ":%s 412 %c :No text to send",
    ":%s 431 %c :No nickname given",
    ":%s 432 %c %n :Erroneus nickname",
    ":%s 433 %c %n :Nickname is already in use",
    ":%s 441 %c %n %h :They aren't on that channel",
    ":%s 442 %c %h :You're not on that channel",
    ":%s 443 %c %n %h :is already on channel",
    ":%s 451 %c :You have not registered",
    ":%s 461 %c %d :Not enough parameters",
    ":%s 462 %c :You may not reregister",
    ":%s 471 %c %h :Cannot join channel (+l)",
    ":%s 473 %c %h :Cannot join channel (+i)",
    ":%s 475 %c %h :Cannot join channel (+k)",
    ":%s 476 %h :Bad Channel Mask",
    ":%s 482 %c %h :You're not channel operator",
};

/**
 * @brief Construct a new Response Arguments:: Response Arguments object
 *
 * @param client: client object
 * @param channel: channel object
 * @param client_list: list of client objects
 * @param command: command string
 *
 * @return ResponseArguments
 *
 * @note This constructor is used to generate response arguments for the
 * response generator
 */
ResponseArguments::ResponseArguments(const Client& client,
                                     const Channel& channel,
                                     const std::vector<Client*>& client_list,
                                     const std::string& command) {
  client_name = client.nick_name();
  channel_name = channel.name();
  topic = channel.topic();
  command_name = command;
  modestirng = channel.GetModeAsString();

  for (size_t i = 0; i < client_list->size(); i++) {
    if (i != 0) {
      nicknames += JUST1RCE_SPACE;
    }
    nicknames += client_list[i]->nick_name();
  }
}

ResponseGenerator::ResponseGenerator() {
  static bool initialized = false;

  if (initialized == false) {
    response_templates_[RPL_WELCOME] =
        ":%s 001 %c :Welcome to the SHELLDIVERS Network, %c";
    response_templates_[RPL_YOURHOST] =
        ":%s 002 %c :Your host is %s, running version %v";
    response_templates_[RPL_CHANNELMODEIS] = 
	    ":%s 324 %c %h %m";
    response_templates_[RPL_NOTOPIC] = 
	    ":%s 331 %c %h :No topic is set";
    response_templates_[RPL_TOPIC] = 
	    ":%s 332 %c %h :%t";
    response_templates_[RPL_INVITING] = 
	    ":%s 341 %c %n %h"; // Need to check
    response_templates_[RPL_NAMREPLY] = 
	    ":%s 353 %c = %h :%n{ %n}"; // Need to check
    response_templates_[RPL_ENDOFNAMES] = 
	    ":%s 366 %c %h :End of /NAMES list";
    response_templates_[ERR_NOSUCHNICK] =
        ":%s 401 %c <nickname> :No such nick/channel";
    response_templates_[ERR_NOSUCHCHANNEL] = ":%s 403 %c %h :No such channel";
    response_templates_[ERR_CANNOTSENDTOCHAN] =
        ":%s 404 %c %h :Cannot send to channel";
    response_templates_[ERR_TOOMANYCHANNELS] =
        ":%s 405 %c %h :You have joined too many channels";
    response_templates_[ERR_NOORIGIN] = 
	    ":%s 409 %c :No origin specified";
    response_templates_[ERR_NORECIPIENT] =
        ":%s 411 %c :No recipient given (%d)";
    response_templates_[ERR_NOTEXTTOSEND] = 
	    ":%s 412 %c :No text to send";
    response_templates_[ERR_NONICKNAMEGIVEN] = 
	    ":%s 431 %c :No nickname given";
    response_templates_[ERR_ERRONEUSNICKNAME] =
        ":%s 432 %c %n :Erroneus nickname"; // Need to check
    response_templates_[ERR_NICKNAMEINUSE] =
        ":%s 433 %c %n :Nickname is already in use"; // Need to check
    response_templates_[ERR_USERNOTINCHANNEL] =
        ":%s 441 %c %n %h :They aren't on that channel"; // Need to check
    response_templates_[ERR_NOTONCHANNEL] =
        ":%s 442 %c %h :You're not on that channel";
    response_templates_[ERR_USERONCHANNEL] =
        ":%s 443 %c %n %h :is already on channel"; // Need to check
    response_templates_[ERR_NOTREGISTERED] =
        ":%s 451 %c :You have not registered";
    response_templates_[ERR_NEEDMOREPARAMS] =
        ":%s 461 %c %d :Not enough parameters";
    response_templates_[ERR_ALREADYREGISTERED] =
        ":%s 462 %c :You may not reregister";
    response_templates_[ERR_CHANNELISFULL] =
        ":%s 471 %c %h :Cannot join channel (+l)";
    response_templates_[ERR_INVITEONLYCHAN] =
        ":%s 473 %c %h :Cannot join channel (+i)";
    response_templates_[ERR_BADCHANNELKEY] =
        ":%s 475 %c %h :Cannot join channel (+k)";
    response_templates_[ERR_BADCHANMASK] = 
	    ":%s 476 %h :Bad Channel Mask";
    response_templates_[ERR_CHANOPRIVSNEEDED] =
        ":%s 482 %c %h :You're not channel operator";

    initialized = true;
  }
}

ResponseGenerator::~ResponseGenerator() {}

/**
 * @brief Generate response message
 *
 * @param numeric: numeric code
 * @param reg_args: response arguments
 *
 * @return std::string
 *
 * @note This function generates response message based on the numeric code and
 * response arguments
 */
std::string ResponseGenerator::generate_response(
    const int numeric, const struct ResponseArguments& reg_args) {
  std::string response_template = response_templates_[numeric];
  std::string response = "";

  for (size_t i = 0; i < response_template.size(); ++i) {
    if (response_template[i] == '%') {
      switch (response_template[++i]) {
        case 'c':
          response += reg_args.client_name;
          break;
        case 'n':
          response += reg_args.nicknames;
          break;
        case 'h':
          response += reg_args.channel_name;
          break;
        case 't':
          response += reg_args.topic;
          break;
        case 'd':
          response += reg_args.command_name;
          break;
        case 'm':
          response += reg_args.modestring;
          break;
        case 'v':
          response += JUST1RCE_VERSION;
          break;
        case 's':
          response += JUST1RCE_SERVER_NAME;
          break;
      }
    } else {
      response += response_template[i];
    }
  }

  return response;
}

}  // namespace Just1RCe
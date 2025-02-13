#include "../../includes/commands/pingCommandHandler.h"

#include <string.h>

#include <vector>

#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

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
std::vector<int> PingCommandHandler::operator()(const int client_fd, const std::string &message) {
	Parser parser(message);
	Client* client = ContextHolder::GetInstance()->db()->GetClient(client_fd);
	std::vector<int> fd_list;

	if (client == nullptr) {
		return fd_list;
	}

	if (client->nick_name().empty() || client->user_name().empty()) {
		client->SetNumeric(":" + JUST1RCE_SERVER_NAME + " 451 " + client->nick_name() + " :You have not registered");
		fd_list.push_back(client_fd);
		return fd_list;
	}

	std::string token;
	int numeric = parser.ParseCommandPing(&token);
	if (numeric) {
		client->SetSendMessage(":" + JUST1RCE_SERVER_NAME + " 409 " + client->nick_name() + " :No origin specified");
		fd_list.push_back(client_fd);
		return fd_list;
	}
	
	client->SetSendMessage("PONG :" + token);
	fd_list.push_back(client_fd);
	return fd_list;
}


} // namespace Just1RCe


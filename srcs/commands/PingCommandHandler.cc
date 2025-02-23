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

	if (client == NULL) {
		return std::vector<int>();
	}

	// Get token
	std::string token;
	parser.ParseCommandPing(&token);
	
	// Send PONG message
	client->SetSendMessage("PONG :" + token);
	fd_list.push_back(client_fd);
	
	return fd_list;
}

const int PingCommandHandler::CheckPing(const Client& client, const std::string& token) {
	if (token.empty() == true) {
		return ERR_NOORIGIN;
	}

	return IRC_NOERROR;
}


} // namespace Just1RCe


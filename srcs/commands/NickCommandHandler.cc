#include "nickCommandHandler.h"
#include "context_holder.h"
#include "parser.h"

#include <string>

namespace Just1RCe {

NickCommandHandler::NickCommandHandler() {}

NickCommandHandler::~NickCommandHandler() {}

void NickCommandHandler::operator()(const int fd, const std::string& message) {
	std::string new_nickname;
	Just1RCe::Parser parser(message);

	try {
		parser.ParseCommandNick(&new_nickname);
	}
	catch (int) {
		Just1RCe::Client::SetSendMessage()
	}
	// 닉네임이 있는지 432
	std::string old_nickname = Just1RCe::ContextHolder::GetInstance()->db_->GetNickNameByFd(fd);
	// 없는 닉네임이면 그냥 할당
	// 있는 닉네임이면 다른 닉네임과 겹치는지 확인하고 넣기
}

}

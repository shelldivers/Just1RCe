#include "../../includes/commands/nickCommandHandler.h"

#include <string.h>

#include "../../includes/client.h"
#include "../../includes/config.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/numeric.h"
#include "../../includes/parser.h"

namespace Just1RCe {

NickCommandHandler::NickCommandHandler() {}

NickCommandHandler::~NickCommandHandler() {}

std::vector<int> NickCommandHandler::operator()(const int fd,
                                                const std::string& message) {
  DbContext* db = ContextHolder::GetInstance()->db();
  std::string new_nickname;
  int numeric = IRC_NOERROR;
  Parser parser(message);

  numeric = parser.ParseCommandNick(&new_nickname);
  if (numeric) 

  Client* client = db->GetClient(fd);
  // 닉네임이 있는지 432
  std::string old_nickname = client->nick_name();
  // 없는 닉네임이면 그냥 할당하고 메시지 반환 x
  if (old_nickname.empty()) {
    client->set_nick_name(new_nickname);
  }
  // 있는 닉네임이면 다른 닉네임과 겹치는지 확인하고 넣고 변경됨을 다른
  // 클라이언트들에게 알림
  else {
  }
}

}  // namespace Just1RCe

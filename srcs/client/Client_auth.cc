
#include <string>

#include "../../includes/client.h"

namespace Just1RCe {

void Client::PassDone() { is_pass_done_ = true; }

bool Client::IsPassed() const { return is_pass_done_; }

bool Client::IsAuthenticated() const {
  return (is_pass_done_ && !(nick_name_.empty()) && !(user_name_.empty()));
}

}  // namespace Just1RCe


#include "../includes/channel.h"

#include <limits>
#include <stdexcept>
#include <string>

namespace Just1RCe {

Channel::Channel(std::string const &name, std::string const &oprt_name,
                 std::string const key = "")
    : name_(name),
      topic_(""),
      key_(key),
      mod_(JUST1RCE_SRCS_CHANNEL_MOD_DEFAULT),
      max_user_num_(std::numeric_limits<size_t>::max()) {
  // wrong channel name format error
  if (!IS_CHANNEL_GLOBAL && !IS_CHANNEL_LOCAL && !IS_CHANNEL_MOELESS &&
      !IS_CHANNEL_SAFE)
    throw std::runtime_error(JUST1RCE_SRCS_CHANNEL_WRONG_NAME_ERROR);
  user_nicknames_.insert(oprt_name);

  if (!key_.empty()) {
    mod_ |= JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET;
  }
}

Channel::~Channel() {}

// getter
std::string Channel::name() const { return name_; }
std::string Channel::topic() const { return topic_; }
std::string Channel::key() const { return key_; }
size_t Channel::max_user_num() const { return max_user_num_; }
size_t Channel::user_num() const { return user_nicknames_.size(); }

// setter
void Channel::set_topic(std::string const &new_topic) { topic_ = new_topic; }
void Channel::set_key(std::string const &new_key) { key_ = new_key; }
size_t Channel::set_max_user_num(size_t const new_max_user_num) {
  max_user_num_ = new_max_user_num;
}
}  // namespace Just1RCe

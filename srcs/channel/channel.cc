
#include "../includes/channel.h"

#include <limits>
#include <stdexcept>
#include <string>

namespace Just1RCe {

/**
 * @brief constructor of the channel object.
 * @param name name of the channel. First character of the name indicates the
 * type of channel. Empty name must be considered as parsing error.
 * @param oprt_name name of the first user who JOIN the channel. Client wth this
 * name must be masked as operator.
 * @param key password for the channel. if pw is not setted, key must be impty.
 * @throw if 'name' is not formatted properly, throw runtime_exception
 */
Channel::Channel(std::string const &name, std::string const &key = "")
    : name_(name),
      topic_(""),
      key_(key),
      mod_(JUST1RCE_SRCS_CHANNEL_MOD_DEFAULT),
      max_user_num_(std::numeric_limits<size_t>::max()) {
  // wrong channel name format error
  if (!IS_CHANNEL_GLOBAL && !IS_CHANNEL_LOCAL && !IS_CHANNEL_MOELESS &&
      !IS_CHANNEL_SAFE)
    throw std::runtime_error(JUST1RCE_SRCS_CHANNEL_WRONG_NAME_ERROR);

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

// setter
void Channel::set_topic(std::string const &new_topic) { topic_ = new_topic; }
void Channel::set_key(std::string const &new_key) { key_ = new_key; }
size_t Channel::set_max_user_num(size_t const new_max_user_num) {
  max_user_num_ = new_max_user_num;
}
}  // namespace Just1RCe

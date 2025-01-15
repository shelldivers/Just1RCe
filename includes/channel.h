
#ifndef JUST1RCE_SRCS_CHANNEL_H
#define JUST1RCE_SRCS_CHANNEL_H

// channel mode flags
#define JUST1RCE_SRCS_CHANNEL_MOD_INVITE_ONLY 0b0000001      // i
#define JUST1RCE_SRCS_CHANNEL_MOD_MSG_OPRT_ONLY 0b0000010    // m
#define JUST1RCE_SRCS_CHANNEL_MOD_MSG_IN_ONLY 0b0000100      // n
#define JUST1RCE_SRCS_CHANNEL_MOD_TOPIC_OPRT_ONLY 0b0001000  // t
#define JUST1RCE_SRCS_CHANNEL_MOD_KEY_SET 0b0010000          // k
#define JUST1RCE_SRCS_CHANNEL_MOD_USER_LIMIT 0b0100000       // l
#define JUST1RCE_SRCS_CHANNEL_MOD_DEFAULT 0b0001100          // default setting
#define JUST1RCE_SRCS_CHANNEL_MOD_ALL 0b01111111             // all mode setting
#define JUST1RCE_SRCS_CHANNEL_MOD_NONE 0b0                   // no mode setting

// error
#define JUST1RCE_SRCS_CHANNEL_WRONG_NAME_ERROR "Channel : wrong channel name."

#include <set>
#include <string>
#include <utility>

typedef unsigned int ChannelModeMask;

namespace Just1RCe {

/**
 *
 * @brief class for irc channel
 * @details handling channel data, modify channel by authority
 * @author eldeshue
 * @date 2025-01-04
 *
 */
class Channel {
 private:
  Channel();
  Channel(Channel const &other);
  Channel &operator=(Channel const &rhs);

  // channel info
  std::string const name_;
  std::string topic_;
  std::string key_;
  ChannelModeMask mod_;

  // user info
  size_t max_user_num_;

  // channel type information
  inline bool is_channel_global() const;
  inline bool is_channel_local() const;

  // TODO(eldeshue) : time information
  /// TODO(eldeshue) : invite_list_, ban_list_

  // Meta Information : unique data to identify someone in the network

 public:
  // constructor
  explicit Channel(std::string const &name, std::string const &key = "");
  ~Channel();

  // getter
  std::string name() const;
  std::string topic() const;
  std::string key() const;

  size_t max_user_num() const;

  // setter
  // auth check needed
  void set_topic(std::string const &new_topic);
  void set_key(std::string const &new_key);
  size_t set_max_user_num(size_t const new_max_user_num);

  // mod handler, authority check needed
  ChannelModeMask GetMode() const;
  std::string GetModeAsString() const;
  void SetMode(ChannelModeMask const flags);
  void UnsetMode(ChannelModeMask const flags);
  bool CheckMode(ChannelModeMask const flags) const;
};
}  // namespace Just1RCe

#endif

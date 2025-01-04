
#ifndef JUST1RCE_SRCS_CHANNEL_H
#define JUST1RCE_SRCS_CHANNEL_H

// channel mode flags

// error

#include <set>
#include <string>

typedef unsigned int ChannelModeMask;
typedef std::set<std::string>::iterator ChannelUserIter;
typedef std::pair<ChannelUserIter, ChannelUserIter> ChannelUserRange;

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
  size_t user_num_;
  std::set<std::string> user_nicknames_;

  // TODO(eldeshue) : time information
  /// TODO(eldeshue) : invite_list_, ban_list_

  // Meta Information : unique data to identify someone in the network

 public:
  // constructor
  Channel(std::string const &name, std::string const &user_name,
          std::string const key = "");
  ~Channel();

  // getter
  std::string name() const;
  std::string topic() const;
  std::string key() const;

  size_t max_user_num() const;
  size_t user_num() const;

  // setter
  // auth check needed
  void set_topic(std::string const &new_topic);
  void set_key(std::string const &new_key);

  size_t set_max_user_num(size_t const new_max_user_num);
  size_t set_user_num(size_t const new_user_num);

  // mod handler, authority check
  // get mode, set mode, get mode as string, is set?
  ChannelModeMask GetMode() const;
  std::string GetModeAsString() const;
  void SetMode(ChannelModeMask mask);
  void UnsetMode(ChannelModeMask mask);
  bool CheckMode(ChannelModeMask mask) const;

  // user control
  void AddUser(std::string const &target_user_name);
  void DeleteUser(std::string const &target_user_name);
  bool CheckUser(std::string const &target_user_name) const;
  void SetUserAuth(std::string const &, char const auth);
};
}  // namespace Just1RCe

#endif

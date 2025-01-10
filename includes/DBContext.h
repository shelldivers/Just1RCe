
#ifndef JUST1RCE_SRCS_DBCONTEXT_H
#define JUST1RCE_SRCS_DBCONTEXT_H

#include <string>
#include <vector>

namespace Just1RCe {

class Channel;
class Client;

/**
 *
 * @brief interface of Data Base's context
 * @details this class will handle IRC server's resources and it's relationship
 * @author eldeshue
 * @date 2025-01-10
 *
 */
class DBContext {
 public:
  virtual ~DBContext();

  // Client index table
  virtual bool AddClient(Client *user) = 0;
  virtual void DelClientByFd(int const fd) = 0;
  virtual void DelClientByNickname(std::string const &nick_name) = 0;
  virtual void DelClientByUsername(std::string const &user_name) = 0;

  virtual Client *GetClientByFd(int const fd) = 0;
  virtual Client *GetClientByNickname(std::string const &nick_name) = 0;
  virtual Client *GetClientByUsername(std::string const &user_name) = 0;

  // Channel index table
  virtual bool AddChannel(Channel *room) = 0;
  virtual void DelChannel(std::string const &channel_name) = 0;

  virtual Channel *GetChannelByName(std::string const &channel_name) = 0;
  virtual size_t GetUserNumOfChannelByName(std::string const &channel_name) = 0;

  // ClientChannel middle table
  virtual bool JoinClientToChannelByNames(std::string const &client_nick_name,
                                          std::string const &channel_name) = 0;
  virtual std::vector<Channel *> GetChannelsByClientNickName(
      std::string const &client_nick_name) = 0;
  virtual std::vector<Client *> GetClientsByChannelName(
      std::string const &channel_name) = 0;
};
};  // namespace Just1RCe

#endif

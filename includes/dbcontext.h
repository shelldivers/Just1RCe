
#ifndef JUST1RCE_SRCS_DBCONTEXT_H
#define JUST1RCE_SRCS_DBCONTEXT_H

#include <string>
#include <vector>

#include "../includes/client_mode.h"

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
class DbContext {
 public:
  virtual ~DbContext();

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

  // client mode table
  virtual void SetClientMode(std::string const &channel_name,
                             std::string const &client_name,
                             ClientModeMask mask) = 0;
  virtual ClientModeMask GetClientMode(std::string const &channel_name,
                                       std::string const &client_name) = 0;
  virtual void DeleteClientMode(std::string const &channel_name,
                                std::string const &client_name) = 0;
  virtual void DeleteClientModesByClientName(
      std::string const &client_name) = 0;
  virtual void DeleteClientModesByChannelName(
      std::string const &channel_name) = 0;

  // mapping table between channel and client
  virtual bool JoinClientToChannelByNames(std::string const &client_nick_name,
                                          std::string const &channel_name) = 0;
  virtual std::vector<Channel *> GetChannelsByClientNickName(
      std::string const &client_nick_name) = 0;
  virtual std::vector<Client *> GetClientsByChannelName(
      std::string const &channel_name) = 0;
};
};  // namespace Just1RCe

#endif

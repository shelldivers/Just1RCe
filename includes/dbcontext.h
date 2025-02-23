
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
  virtual ~DbContext() {}

  // Client index table
  virtual bool AddClient(Client *user) = 0;
  virtual void DelClient(int const client_fd) = 0;

  virtual Client *GetClient(int const client_fd) = 0;

  // Channel index table
  virtual bool AddChannel(Channel *room) = 0;
  virtual void DelChannel(std::string const &channel_name) = 0;

  virtual Channel *GetChannel(std::string const &channel_name) = 0;
  virtual size_t GetNumOfClientInChannel(std::string const &channel_name) = 0;

  // client mode table
  virtual void SetClientMode(int const client_fd,
                             std::string const &channel_name,
                             ClientModeMask mask) = 0;
  virtual ClientModeMask GetClientMode(int const client_fd,
                                       std::string const &channel_name) = 0;
  virtual void DeleteClientMode(int const client_fd,
                                std::string const &channel_name) = 0;
  virtual void DeleteClientModesByClientFd(int const client_fd) = 0;

  // mapping table between channel and client
  virtual bool JoinClientToChannel(int const client_fd,
                                   std::string const &channel_name) = 0;
  virtual void PartClientFromChannel(int const client_fd,
                                     std::string const &channel_name) = 0;
  virtual std::vector<Channel *> GetChannelsByClientFd(int const client_fd) = 0;
  virtual std::vector<Client *> GetClientsByChannelName(
      std::string const &channel_name) = 0;

  // nick name to fd mapping table
  virtual void SetNickNameToFd(std::string const &client_nick_name,
                               int const fd) = 0;
  virtual int GetFdByNickName(std::string const &client_nick_name) = 0;
  virtual void DeleteNickNameToFd(std::string const &client_nick_name) = 0;
};

};  // namespace Just1RCe

#endif

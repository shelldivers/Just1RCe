
#ifndef JUST1RCE_SRCS_IN_MEMORY_DB_CONTEXT_H
#define JUST1RCE_SRCS_IN_MEMORY_DB_CONTEXT_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "../includes/channel.h"
#include "../includes/client.h"
#include "../includes/client_mode.h"
#include "../includes/dbcontext.h"

namespace Just1RCe {

// type alias
typedef std::string ClientNickName;
typedef std::string ClientUserName;
typedef std::string ChannelName;
typedef std::pair<ClientNickName, ChannelName> ModeKey;
typedef int Fd;

typedef std::map<ClientNickName, Client *>::iterator ClientTableIter;
typedef std::map<ChannelName, Channel *>::iterator ChannelTableIter;
typedef std::multimap<std::string, std::string>::iterator MappingTableIter;
typedef std::map<ModeKey, ClientModeMask>::iterator ModeTableIter;

class InMemoryDbContext : public DbContext {
 private:
  InMemoryDbContext(InMemoryDbContext const &);
  InMemoryDbContext &operator=(InMemoryDbContext const &);

  // index table, 1:1
  std::map<ClientNickName, Client *> client_table_;
  std::map<ChannelName, Channel *> channel_table_;
  std::map<ModeKey, ClientModeMask> mode_table_;

  // auxillary table, 1:1
  std::map<Fd, ClientNickName> fd_to_client_mapping_table_;

  // mapping table, n:n
  std::multimap<ClientNickName, ChannelName> client_to_channel_mapping_table_;
  std::multimap<ChannelName, ClientNickName> channel_to_client_mapping_table_;

 public:
  InMemoryDbContext();
  ~InMemoryDbContext();

  // Client index table
  virtual bool AddClient(Client *user);
  virtual void DelClient(std::string const &client_nick_name);

  virtual Client *GetClient(std::string const &client_nick_name);
  virtual std::string GetNickNameByFd(int const fd);

  // Channel index table
  virtual bool AddChannel(Channel *room);
  virtual void DelChannel(std::string const &channel_name);

  virtual Channel *GetChannel(std::string const &channel_name);
  virtual size_t GetNumOfClientInChannel(std::string const &channel_name);

  // client mode table
  virtual void SetClientMode(std::string const &client_nick_name,
                             std::string const &channel_name,
                             ClientModeMask mask);
  virtual ClientModeMask GetClientMode(std::string const &client_nick_name,
                                       std::string const &channel_name);
  virtual void DeleteClientMode(std::string const &client_nick_name,
                                std::string const &channel_name);
  virtual void DeleteClientModesByClientName(
      std::string const &client_nick_name);

  // mapping table between channel and client
  virtual bool JoinClientToChannel(std::string const &client_nick_name,
                                   std::string const &channel_name);
  virtual void PartClientFromChannel(std::string const &client_nick_name,
                                     std::string const &channel_name);
  virtual std::vector<Channel *> GetChannelsByClientNickName(
      std::string const &client_nick_name);
  virtual std::vector<Client *> GetClientsByChannelName(
      std::string const &channel_name);
};

}  // namespace Just1RCe

#endif

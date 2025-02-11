
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
typedef int Fd;
typedef std::string ChannelName;
typedef std::pair<Fd, ChannelName> ModeKey;

typedef std::map<Fd, Client *>::iterator ClientTableIter;
typedef std::map<ChannelName, Channel *>::iterator ChannelTableIter;
typedef std::multimap<Fd, std::string>::iterator
    ClientToChannelMappingTableIter;
typedef std::multimap<std::string, Fd>::iterator
    ChannelToClientMappingTableIter;
typedef std::map<ModeKey, ClientModeMask>::iterator ModeTableIter;

class InMemoryDbContext : public DbContext {
 private:
  InMemoryDbContext(InMemoryDbContext const &);
  InMemoryDbContext &operator=(InMemoryDbContext const &);

  // index table, 1:1
  std::map<Fd, Client *> client_table_;
  std::map<ChannelName, Channel *> channel_table_;
  std::map<ModeKey, ClientModeMask> mode_table_;

  // mapping table, n:n
  std::multimap<Fd, ChannelName> client_to_channel_mapping_table_;
  std::multimap<ChannelName, Fd> channel_to_client_mapping_table_;

 public:
  InMemoryDbContext();
  ~InMemoryDbContext();

  // Client index table
  virtual bool AddClient(Client *user);
  virtual void DelClient(int const client_fd);

  virtual Client *GetClient(int const client_fd);

  // Channel index table
  virtual bool AddChannel(Channel *room);
  virtual void DelChannel(std::string const &channel_name);

  virtual Channel *GetChannel(std::string const &channel_name);
  virtual size_t GetNumOfClientInChannel(std::string const &channel_name);

  // client mode table
  virtual void SetClientMode(int const client_fd,
                             std::string const &channel_name,
                             ClientModeMask mask);
  virtual ClientModeMask GetClientMode(int const client_fd,
                                       std::string const &channel_name);
  virtual void DeleteClientMode(int const client_fd,
                                std::string const &channel_name);
  virtual void DeleteClientModesByClientFd(int const client_fd);

  // mapping table between channel and client
  virtual bool JoinClientToChannel(int const client_fd,
                                   std::string const &channel_name);
  virtual void PartClientFromChannel(int const client_fd,
                                     std::string const &channel_name);
  virtual std::vector<Channel *> GetChannelsByClientFd(int const client_fd);
  virtual std::vector<Client *> GetClientsByChannelName(
      std::string const &channel_name);
};

}  // namespace Just1RCe

#endif

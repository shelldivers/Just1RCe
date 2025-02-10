
#include <string>
#include <utility>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

/**
 *
 * @brief add client to the table
 * @warning caller must check user's nick name is valid before creating client
 * @return return false if the value of user is NULL
 */
bool InMemoryDbContext::AddClient(Client *user) {
  // sanity check
  if (user == NULL) return false;

  // add index table
  client_table_.insert(std::make_pair(user->nick_name(), user));

  // add fd to nick name
  fd_to_client_mapping_table_.insert(
      std::make_pair(user->GetFd(), user->nick_name()));

  return true;
}

/**
 *
 * @brief delete client and it's related data
 */
void InMemoryDbContext::DelClient(std::string const &client_nick_name) {
  // sanity check
  if (!client_table_.count(client_nick_name)) return;

  // get target
  Client *erase_target = client_table_[client_nick_name];

  // erase fd to nickname mapping table
  fd_to_client_mapping_table_.erase(erase_target->GetFd());

  // delete client's modes with 'nick_name'
  DeleteClientModesByClientName(client_nick_name);

  // delete client
  delete erase_target;
  client_table_.erase(client_nick_name);
}

Client *InMemoryDbContext::GetClient(std::string const &client_nick_name) {
  if (!client_table_.count(client_nick_name)) return NULL;
  return client_table_[client_nick_name];
}

/**
 * @return if the fd is invalid, return ""
 */
std::string InMemoryDbContext::GetNickNameByFd(int const fd) {
  // sanity check
  if (!fd_to_client_mapping_table_.count(fd)) return "";
  return fd_to_client_mapping_table_[fd];
}

}  // namespace Just1RCe


#include "../../includes/in_memory_db_context.h"

#include <algorithm>

namespace Just1RCe {

InMemoryDbContext::InMemoryDbContext() {}
InMemoryDbContext::~InMemoryDbContext() {
  // delete client table
  for (ClientTableIter itr = client_table_.begin(); itr != client_table_.end();
       ++itr) {
    // delete pointer to client
    delete itr->second;
  }

  // delete channel table
  for (ChannelTableIter itr = channel_table_.begin();
       itr != channel_table_.end(); ++itr) {
    // delete pointer to channel
    delete itr->second;
  }
}

}  // namespace Just1RCe

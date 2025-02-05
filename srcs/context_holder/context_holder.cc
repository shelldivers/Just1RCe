

#include "../../includes/context_holder.h"

#include <cstdlib>

#include "../../includes/in_memory_db_context.h"

namespace Just1RCe {

ContextHolder *ContextHolder::g_instance_ptr = NULL;

ContextHolder::ContextHolder() : db_(new InMemoryDbContext()) {}

ContextHolder::~ContextHolder() { delete db_; }

/**
 * @brief pointer to the object, only access way to signelton object
 * @warning do not allocate it's return value.
 */
ContextHolder *ContextHolder::GetInstance() {
  if (g_instance_ptr == NULL) g_instance_ptr = new ContextHolder();
  return g_instance_ptr;
}

DbContext *ContextHolder::db() { return db_; }

}  // namespace Just1RCe


#include "../../includes/context_holder.h"

#include <cstdlib>

namespace Just1RCe {

explicit ContextHolder::ContextHolder(DbContext *ptr) : db_(ptr) {}

ContextHolder::~ContextHolder() { delete db_; }

ContextHolder *ContextHolder::GetInstance(DbContext *ptr) {
  if (g_instance_ptr == NULL) g_instance_ptr = new ContextHolder(ptr);
  return g_instance_ptr;
}

DbContext *ContextHolder::db() { return db_; }

}  // namespace Just1RCe


#include "../../includes/context_holder.h"

#include <cstdlib>

namespace Just1RCe {

ContextHolder *ContextHolder::g_instance_ptr = NULL;

explicit ContextHolder::ContextHolder() : db_(NULL) {}

ContextHolder::~ContextHolder() { delete db_; }

/**
 * @brief pointer to the object, only access way to signelton object
 * @warning do not allocate it's return value.
 */
ContextHolder *ContextHolder::GetInstance() {
  if (g_instance_ptr == NULL) g_instance_ptr = new ContextHolder();
  return g_instance_ptr;
}

/**
 * @param db pointer to the DbContext, DbContext is interface, polymorphic
 * @warning mulst be called once
 */
void ContextHolder::InitDbContext(DbContext *db) { db_ = db; }

DbContext *ContextHolder::db() { return db_; }

}  // namespace Just1RCe

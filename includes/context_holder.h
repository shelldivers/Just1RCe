
#ifndef CONTEXT_HOLDER_H
#define CONTEXT_HOLDER_H

namespace Just1RCe {

class DbContext;

/**
 * @date 2025-02-03
 * @brief class that handle DbContext, singleton pattern
 * @author eldeshue
 */
class ContextHolder {
 private:
  ContextHolder();
  ContextHolder(ContextHolder const &);
  ~ContextHolder();

  ContextHolder &operator=(ContextHolder const &);

  static ContextHolder *g_instance_ptr;
  DbContext *db_;

 public:
  static ContextHolder *GetInstance();

  void InitDbContext(DbContext *db);
  DbContext *db();
};

}  // namespace Just1RCe

#endif

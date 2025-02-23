
#ifndef JUST1RCE_SRCS_SERVICE_H
#define JUST1RCE_SRCS_SERVICE_H

#define JUST1RCE_SRCS_EVENT_BUFFER_SIZE 1024

#include <map>
#include <string>

namespace Just1RCe {

class CommandHandler;
typedef std::map<std::string, CommandHandler *> CommandMapping;

class Service {
 private:
  Service();
  Service(Service const &);
  ~Service();
  Service &operator=(Service const &);

  // event handler
  static void HandleServerEvent(int const epoll_fd,
                                CommandMapping const &cmd_map,
                                struct epoll_event const &cur_event);
  static void HandleClientEvent(int const epoll_fd,
                                CommandMapping const &cmd_map,
                                struct epoll_event const &cur_event);
  static void RunCommand(int const epoll_fd, CommandMapping const &cmd_map,
                         struct epoll_event const &cur_event,
                         std::string const &msg);

  // event utility
  static void RegisterServer(int const epoll_fd, int const server_fd);
  static void RegisterClient(int const epoll_fd, int const client_fd);
  static void UnRegisterClient(int const epoll_fd, int const client_fd);
  static void SetWriteEvent(int const epoll_fd, int const target_fd);
  static void SetReadEvent(int const epoll_fd, int const target_fd);

 public:
  static void RunIrcServer(std::string const &port_number,
                           CommandMapping const &cmd_map);
};

}  // namespace Just1RCe

#endif

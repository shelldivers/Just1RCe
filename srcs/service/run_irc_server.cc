
extern "C" {
#include <sys/epoll.h>
}

#include <stdexcept>
#include <string>

#include "../../includes/command_handler.h"
#include "../../includes/service.h"
#include "../../includes/tcp_socket.h"

namespace Just1RCe {

/**
 *
 * @brief service routine of the irc server
 * @warning because of using epoll, it works only on Linux
 * @param port_number port number in string, to make server socket
 * @param cmd_map map command string to command handler
 *
 */
void Service::RunIrcServer(std::string const &port_number,
                           CommandMapping const &cmd_map) {
  // get kernel event queue, epoll
  int const epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) throw std::runtime_error("epoll creation failed");
  struct epoll_event event_buffer[JUST1RCE_SRCS_EVENT_BUFFER_SIZE];

  // get server socket(listening socket)
  TcpSocket server(port_number);
  RegisterServer(epoll_fd, server.socket_fd());

  while (true) {
    // get events from event queue
    int const recv_event_cnt =
        epoll_wait(epoll_fd, event_buffer, JUST1RCE_SRCS_EVENT_BUFFER_SIZE, -1);
    if (recv_event_cnt == -1) throw std::runtime_error("epoll_wait failed");

    // handling events
    for (int i = 0; i < recv_event_cnt; ++i) {
      if (event_buffer[i].data.fd == server.socket_fd()) {
        HandleServerEvent(epoll_fd, event_buffer[i]);
      } else {
        try {
          HandleClientEvent(epoll_fd, cmd_map, event_buffer[i]);
        } catch (const std::exception &e) {
          RunCommand(epoll_fd, cmd_map, event_buffer[i],
                     "QUIT :unexpected connection destroyed");
        }
      }
    }
  }
}

}  // namespace Just1RCe

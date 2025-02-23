
extern "C" {
#include <sys/epoll.h>
}

#include <stdexcept>

#include "../../includes/service.h"

namespace Just1RCe {

void Service::RegisterServer(int const epoll_fd, int const server_fd) {
  struct epoll_event event;

  event.events = EPOLLIN | EPOLLET;
  event.data.fd = server_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1)
    throw std::runtime_error("event register failed");
}

void Service::RegisterClient(int const epoll_fd, int const client_fd) {
  struct epoll_event event;

  event.events = EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
  event.data.fd = client_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
    throw std::runtime_error("event register failed");
}

void Service::UnRegisterClient(int const epoll_fd, int const client_fd) {
  if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL) == -1)
    throw std::runtime_error("event register failed");
}

void Service::SetWriteEvent(int const epoll_fd, int const target_fd) {
  struct epoll_event event;

  event.events = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
  event.data.fd = target_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, target_fd, &event) == -1)
    throw std::runtime_error("event register failed");
}

void Service::SetReadEvent(int const epoll_fd, int const target_fd) {
  struct epoll_event event;

  event.events = EPOLLIN | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
  event.data.fd = target_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, target_fd, &event) == -1)
    throw std::runtime_error("event register failed");
}

}  // namespace Just1RCe

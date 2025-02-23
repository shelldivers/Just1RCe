
extern "C" {
#include <sys/epoll.h>
}

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../includes/client.h"
#include "../../includes/command_handler.h"
#include "../../includes/context_holder.h"
#include "../../includes/dbcontext.h"
#include "../../includes/ft_optional.h"
#include "../../includes/service.h"
#include "../../includes/tcp_socket.h"

namespace Just1RCe {

void Service::HandleServerEvent(struct epoll_event const &cur_event) {
  // check sanity of server socket
  if (cur_event.events & (EPOLLHUP | EPOLLERR))
    throw std::runtime_error("server socket is broken");

  // add new user
  Client *new_user = new Client(cur_event.data.fd);
  ContextHolder::GetInstance()->db()->AddClient(new_user);
}

static std::string GetCommandFromMessage(std::string const &msg) {
  std::stringstream ss(msg);
  std::string first_word;
  ss >> first_word;
  return first_word;
}

void Service::RunCommand(int const epoll_fd, CommandMapping const &cmd_map,
                         struct epoll_event const &cur_event,
                         std::string const &msg) {
  // call command handler
  std::string const command = GetCommandFromMessage(msg);
  std::vector<int> const fd_to_write =
      (*(cmd_map.find(command)->second))(cur_event.data.fd, msg);

  // set all messages
  for (int i = 0; i < fd_to_write.size(); ++i)
    SetWriteEvent(epoll_fd, fd_to_write[i]);
}

void Service::HandleClientEvent(int const epoll_fd,
                                CommandMapping const &cmd_map,
                                struct epoll_event const &cur_event) {
  if (cur_event.events & (EPOLLERR | EPOLLRDHUP | EPOLLHUP)) {
    // un expected connection broke
    UnRegisterClient(epoll_fd, cur_event.data.fd);
    RunCommand(epoll_fd, cmd_map, cur_event,
               "QUIT :unexpected connection destroyed");
    return;
  }

  // get target client
  Client *const target =
      ContextHolder::GetInstance()->db()->GetClient(cur_event.data.fd);

  // read event
  if (cur_event.events & EPOLLIN) {
    ft::optional<std::vector<std::string> > messages =
        target->GetReceivedMessages();
    SetReadEvent(epoll_fd, cur_event.data.fd);

    // run commands
    if (messages.has_value()) {
      for (int i = 0; i < (*messages).size(); ++i) {
        RunCommand(epoll_fd, cmd_map, cur_event, (*messages)[i]);
      }
    }
  }

  // write event
  if (cur_event.events & EPOLLOUT) {
    bool const do_write_again = target->SendMessage();

    if (do_write_again) {
      SetWriteEvent(epoll_fd, cur_event.data.fd);
    } else {
      SetReadEvent(epoll_fd, cur_event.data.fd);
    }
  }
}

}  // namespace Just1RCe

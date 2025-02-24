
#include <iostream>
#include <map>
#include <string>

#include "../includes/command_handler.h"
#include "../includes/command_mapping.h"
#include "../includes/service.h"

int main(int argc, char* argv[]) {
  // input check
  if (argc != 3) {
    std::cerr << "Error : wrong number of arguments\n";
    return -1;
  }

  Just1RCe::CommandMapping cmd_map;
  Just1RCe::RegistCommandHandlers(&cmd_map, argv[2]);

  try {
    Just1RCe::Service::RunIrcServer(argv[1], cmd_map);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
  }
  return 0;
}

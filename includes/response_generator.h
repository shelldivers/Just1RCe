#ifndef JUST1RCE_SRCS_RESPONSE_GENERATOR_H_
#define JUST1RCE_SRCS_RESPONSE_GENERATOR_H_

#include <string>
#include <vector>

class Client;
class Channel;

namespace Just1RCe {

struct ResponseArguments {
  std::string client_name;   // %c, 클라이언트 이름
  std::string nicknames;     // %n, 닉네임 목록 (optional)
  std::string channel_name;  // %h, 채널 (optional)
  std::string topic;         // %t, 채널 주제 ((optional))
  std::string command_name;  // %d, 명령어
  std::string modstring;     // %m, 모드 문자열 (optional)

  ResponseArguments(const Client& client, const Channel& channel,
                    const std::vector<Client*>& client_list,
                    const std::string& command);
};

class ResponseGenerator {
 private:
  static const char* response_templates_[1000];

 public:
  ResponseGenerator();
  ~ResponseGenerator();
  std::string generate_response(const int numeric,
                                const struct ResponseArguments& res_args);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_SRCS_RESPONSE_GENERATOR_H_

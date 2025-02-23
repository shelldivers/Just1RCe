#ifndef JUST1RCE_SRCS_RESPONSE_GENERATOR_H_
#define JUST1RCE_SRCS_RESPONSE_GENERATOR_H_

#include <string>
#include <vector>

class Client;
class Channel;

#define JUST1RCE_MAX_NUMERIC 1000

namespace Just1RCe {

struct ResponseArguments {
  std::string client_name;          // %c, 클라이언트 이름
  std::string channel_name;         // %h, 채널 (optional)
  std::string nicknames;            // %n, 닉네임 (optional)
  std::string topic;                // %t, 채널 주제 (optional)
  std::string modestring;           // %m, 모드 문자열 (optional)
  std::string command_name;         // %C, 명령어 이름
  std::vector<std::string> params;  // %p, 파라미터 목록

  ResponseArguments(const int numeric, const Client& client,
                    const Channel* channel,
                    const std::vector<std::string>& params);
};

class ResponseGenerator {
 private:
  const char* response_templates_[JUST1RCE_MAX_NUMERIC];

  ResponseGenerator();
  ResponseGenerator(const ResponseGenerator&);
  ResponseGenerator& operator=(const ResponseGenerator&);
  void InitializeTemplates();

 public:
  static ResponseGenerator& GetInstance();
  ~ResponseGenerator();
  std::string GenerateResponse(const int numeric,
                               const struct ResponseArguments& res_args);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_SRCS_RESPONSE_GENERATOR_H_

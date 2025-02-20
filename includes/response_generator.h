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
  std::string modstring;            // %m, 모드 문자열 (optional)
  std::vector<std::string> params;  // %p, 파라미터 목록

  ResponseArguments(const int numeric, const Client& client,
                    const Channel* channel, std::vector<std::string> params);
};

class ResponseGenerator {
 private:
  static ResponseGenerator* instance_;
  const char* response_templates_[JUST1RCE_MAX_NUMERIC];

  ResponseGenerator();
  ResponseGenerator(const ResponseGenerator&);
  ResponseGenerator& operator=(const ResponseGenerator&);
  void InitializeTemplates();

 public:
  static ResponseGenerator* GetInstance();
  static void DestroyInstance();
  ~ResponseGenerator();
  std::string GenerateResponse(const int numeric,
                               const struct ResponseArguments& res_args);
};

}  // namespace Just1RCe

#endif  // JUST1RCE_SRCS_RESPONSE_GENERATOR_H_

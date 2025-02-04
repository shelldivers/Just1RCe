#ifndef JUST1RCE_INCLUDES_LEXER_H
#define JUST1RCE_INCLUDES_LEXER_H

#include <vector>
#include <string>

#define JUST1RCE_MAX_MIDDLE_SIZE 14
#define JUST1RCE_SPACE ' '
#define JUST1RCE_COLON ':'

namespace Just1RCe {

std::vector<std::string> CreateTokenStream(const std::string &message);

} // namespace Just1RCe


#endif // JUST1RCE_INCLUDES_PARSE_H
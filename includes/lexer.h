#ifndef JUST1RCE_INCLUDES_LEXER_H
#define JUST1RCE_INCLUDES_LEXER_H

#include <vector>
#include <string>

#define JUST1RCE_MAX_MIDDLE_SIZE 14
#define JUST1RCE_SPACE ' '
#define JUST1RCE_COLON ':'

namespace Just1RCe {

/**
 *
 * @brief A struct that breaks down text into tokens
 * @details This struct lexing message from string and store it in member variables
 * @author jiwojung
 * @date 2025-02-04
 */
struct TokenStream {
	std::string command_;
	std::vector<std::string> parameters_;

	TokenStream(const std::string& message);
	~TokenStream();

private:
	TokenStream(const TokenStream& ts);
	TokenStream& operator=(const TokenStream& ts); 
};

} // namespace Just1RCe


#endif // JUST1RCE_INCLUDES_PARSE_H
#ifndef JUST1RCE_INCLUDES_PARSE_H
#define JUST1RCE_INCLUDES_PARSE_H

#include <vector>
#include <string>

#define JUST1RCE_MAX_MIDDLE_SIZE 14
#define JUST1RCE_SPACE ' '
#define JUST1RCE_COLON ':'

namespace Just1RCe {

/**
 *
 * @brief Class that parse message from buffer
 * @details This class parse message from buffer and store it in member variables
 * @author jiwojung
 * @date 2025-02-04
 */
struct Parser {
	std::string command_;
	std::string middle_[JUST1RCE_MAX_MIDDLE_SIZE];
	std::string trailing_;	
	size_t middle_size_;
	bool is_trailing_;

	void parse(const std::string& message);
};

} // namespace Just1RCe


#endif // JUST1RCE_INCLUDES_PARSE_H
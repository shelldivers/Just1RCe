#include "../../includes/parser.h"
#include <vector>
#include <string>

namespace Just1RCe {

void Parser::parse(const std::string& message) {
	if (message.empty()) return;

	middle_size_ = 0;
	is_trailing_ = false;
	int before_index = 0;

	for (int current_index = 0; current_index < message.size(); ++current_index) {
		if (message[current_index] == JUST1RCE_SPACE || current_index == message.size() - 1) {
			if (before_index) {
				middle_[middle_size_++] = message.substr(before_index + 1, current_index - before_index - 1);
				before_index = current_index;
			} else {
				command_ = message.substr(0, current_index);
				before_index = current_index;
			}
		}
		else if (message[current_index] == JUST1RCE_COLON) {
			is_trailing_ = true;
			trailing_ = message.substr(current_index + 1);
			break;
		}
	}
}	

}
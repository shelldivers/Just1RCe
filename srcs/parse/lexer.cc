#include "../../includes/lexer.h"
#include <string>
#include <vector>

namespace Just1RCe
{

TokenStream::TokenStream(const std::string &message)
{
    if (message.empty())
        return;

    int current_index = 0;
    int before_index;

    while (message[current_index++] != JUST1RCE_SPACE)
        ;
    command_ = message.substr(0, current_index - 1);
    before_index = current_index - 1;

    while (current_index++ < message.size() && parameters_.size() < JUST1RCE_MAX_MIDDLE_SIZE)
    {
        if (message[current_index] == JUST1RCE_SPACE)
        {
            parameters_.push_back(message.substr(before_index + 1, current_index - before_index - 1));
            before_index = current_index;
        }
        else if (message[current_index] == JUST1RCE_COLON)
        {
            parameters_.push_back(message.substr(current_index + 1));
            return;
        }
    }

	if (parameters_.size() == JUST1RCE_MAX_MIDDLE_SIZE) 
	{
		size_t colon_pos = message.find(':');
		if (colon_pos != std::string::npos)
			parameters_.push_back(message.substr(colon_pos + 1));
	}
	else 
	{
    	parameters_.push_back(message.substr(before_index + 1, current_index - before_index));
	}
}

TokenStream::~TokenStream()
{
}

} // namespace Just1RCe

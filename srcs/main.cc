#include "../includes/parser.h"
#include <iostream>



int main() {
    Just1RCe::Parser parser;
    parser.parse("JOIN #a,#b,#c 111,222,333");

    std::cout << "command: " << parser.command_ << std::endl;

    for (int i = 0; i < parser.middle_size_; ++i) {
        std::cout << "middle: " << parser.middle_[i] << std::endl;
    }

    std::cout << "trailing: ";
    std::cout << parser.trailing_ << std::endl;
    return 0;
}

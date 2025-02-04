#include "../includes/parser.h"
#include <iostream>

void printParsed(Just1RCe::Parser& parser) {
    std::cout << "command :" << parser.command_ << std::endl << std::endl;

    for (int i = 0; i < parser.middle_size_; ++i) {
        std::cout << "middle :" << parser.middle_[i] << std::endl;
    }
    std::cout << std::endl;

    std::cout << "trailing :" << parser.trailing_ << std::endl;
}

int main() {
    Just1RCe::Parser parser;
    parser.parse("JOIN #a,#b,#c 111,222,333");
    printParsed(parser);

    parser.parse("NICK :new_nickname");
    printParsed(parser);
    return 0;
}

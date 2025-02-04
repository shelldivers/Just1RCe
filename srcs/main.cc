#include "../includes/parse.h"
#include <iostream>

void PrintTokenStream(std::vector<std::string> TokenStream) {
    std::cout << "command :";
    std::cout << TokenStream[0] << std::endl;

    std::cout << "params :" << std::endl;
    for (int i = 1; i < TokenStream.size(); ++i) {
        std::cout << TokenStream[i] << std::endl;
    }
}

int main() {
    std::vector<std::string> TokenStream = Just1RCe::CreateTokenStream("JOIN #a,#b,#c 111,222,333");
    PrintTokenStream(TokenStream);

    std::vector<std::string> TokenStream2 = Just1RCe::CreateTokenStream("NICK :new_nickname");
    PrintTokenStream(TokenStream2);

    std::vector<std::string> TokenStream3 = Just1RCe::CreateTokenStream("NICK aaa aa a a a a a a a a a a a a a a a a a a a a a :b");
    PrintTokenStream(TokenStream3);
    return 0;
}

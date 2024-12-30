#include "../includes/math.h"
#include "../includes/config.h"

#include <iostream>

int main(int argc, char *argv[]) {
	int a;
	Math math(3, 15);

	std::cout << PROJECT_VERSION << std::endl;

	std::cout << math.add() << std::endl;
	std::cout << math.sub() << std::endl;
	std::cout << math.mul() << std::endl;
	std::cout << math.div() << std::endl;
}
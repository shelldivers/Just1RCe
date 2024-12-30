#ifndef TEST_MATH_H
#define TEST_MATH_H

class Math {
	int a_;
	int b_;

public:
	Math(int a, int b);

	int add();
	int sub();
	int mul();
	int div();
};

#endif
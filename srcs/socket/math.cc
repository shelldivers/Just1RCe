#include "../includes/math.h"

Math::Math(int a, int b) : a_(a), b_(b) {
	
}

int Math::add(void) {
	return a_ + b_;
}

int Math::sub(void) {
	return a_ - b_;
}

int Math::mul(void) {
	return a_ * b_;
}

int Math::div(void) {
	if (a_ && b_)
		return a_ / b_;
	return 0;
}
#include "Utils.h"

int summation(int n) {
	return n*(n + 1) / 2;
}

int reverseSummation(int sum) {
	return sqrt(8 * sum + 1) / 2;
}

double cubicBezierCurve(double x, double p1, double p2, double p3, double p4) {
	return p1*x*x*x + (3 * p2)*x*x*(1 - x) + (3 * p3)*x*(1 - x)*(1 - x) + p4*(1 - x)*(1 - x)*(1 - x);
}

double cubicBezierEasing(double change, double delta, double duration) {
	return change*cubicBezierCurve(delta / duration, 1, 0.55, 0.45, 0);
}

double linearEasing(double change, double delta, double duration) {
	return change * (delta / duration);
}

double quadraticEasingInOut(double change, double delta, double duration) {
	delta /= duration / 2;
	if (delta < 1) return change / 2 * delta*delta;
	delta--;
	return -change / 2 * (delta*(delta - 2) - 1);
}

void setBit(uint32_t& x, unsigned bit) {
	x |= 1 << bit;
}

void clearBit(uint32_t& x, unsigned bit) {
	x &= ~(1 << bit);
}

void toggleBit(uint32_t& x, unsigned bit) {
	x ^= 1 << bit;
}

bool getBit(const uint32_t& x, unsigned bit) {
	return x & (1 << bit);
}
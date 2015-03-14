#include "Utils.h"

int summation(int n) {
	return n*(n + 1) / 2;
}

int reverseSummation(int sum) {
	return sqrt(8 * sum + 1) / 2;
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
#pragma once
#include "Main.h"

#define C_PI 3.14159265358979323846

int summation(int n);

int reverseSummation(int sum);

double cubicBezierCurve(double x, double p1, double p2, double p3, double p4);

double cubicBezierEasing(double change, double delta, double duration);

double linearEasing(double change, double delta, double duration);

double quadraticEasingInOut(double change, double delta, double duration);

void setBit(uint32_t& x, unsigned bit);

void clearBit(uint32_t& x, unsigned bit);

void toggleBit(uint32_t& x, unsigned bit);

bool getBit(const uint32_t& x, unsigned bit);



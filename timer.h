#pragma once
#include "main.h"

class TTimer {
    unsigned int _ticks;
    unsigned int _maxTicks;
    TTimer();
public:
    TTimer(unsigned int maxTicks);
    ~TTimer();
    bool check();
    void relaunch();
    bool isActive();
    unsigned int getTicks();
    void setTicks(unsigned int ticks);
};
#include "timer.h"

TTimer::TTimer(){
    _ticks = 0;
}

TTimer::TTimer(unsigned maxTicks){
    _maxTicks = maxTicks;
    _ticks = 0;
}

TTimer::~TTimer(){

}

/* TTimer::check() function to check the timer in control loop
 * decrements the counter
 * returns true if timer is finished
*/
bool TTimer::check(){
    if(_ticks != 0)
        _ticks--;
    return !_ticks;
}

void TTimer::relaunch(){
    _ticks = _maxTicks;
}

bool TTimer::isActive(){
    return _ticks;
}

unsigned TTimer::getTicks(){
    return _ticks;
}

void TTimer::setTicks(unsigned ticks){
    _ticks = ticks;
}
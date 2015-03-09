#include "timer.h"

TaskTimer::TaskTimer() {
	_ticks = 0;
}

TaskTimer::TaskTimer(unsigned maxTicks) {
	_ticks = _maxTicks = maxTicks;
}

TaskTimer::TaskTimer(const TaskTimer& rhs) {
	_ticks = rhs._ticks;
	_maxTicks = rhs._maxTicks;
}

TaskTimer::~TaskTimer(){

}

TaskTimer& TaskTimer::operator= (const TaskTimer& rhs) {
		_ticks = rhs._ticks;
		_maxTicks = rhs._maxTicks;
}

/* TaskTimer::check() function to check the timer in control loop
 * decrements the counter
 * returns true if timer is finished
 */
bool TaskTimer::check() {
	if (_ticks != 0)
		_ticks--;
	return !_ticks;
}

void TaskTimer::relaunch() {
	_ticks = _maxTicks;
}

bool TaskTimer::isActive() const {
	return _ticks;
}

unsigned TaskTimer::getTicks() const {
	return _ticks;
}

void TaskTimer::setTicks(unsigned ticks) {
	_ticks = ticks;
}

void TaskTimer::adjust(unsigned newMaximum) {
	_maxTicks = newMaximum;
	if (_ticks > _maxTicks) {
		_ticks = _maxTicks;
	}
}
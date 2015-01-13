#pragma once

class TaskTimer {
	unsigned int _ticks;
	unsigned int _maxTicks;
public:
	TaskTimer();
	TaskTimer(unsigned int maxTicks);
	~TaskTimer();
	bool check();
	void relaunch();
	bool isActive();
	unsigned int getTicks();
	void setTicks(unsigned int ticks);
	void adjust(unsigned int newMaximum);
};
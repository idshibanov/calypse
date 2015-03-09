#pragma once

class TaskTimer {
	unsigned int _ticks;
	unsigned int _maxTicks;
public:
	TaskTimer();
	TaskTimer(const TaskTimer& rhs);
	TaskTimer(unsigned int maxTicks);
	~TaskTimer();
	TaskTimer& operator= (const TaskTimer& rhs);
	bool check();
	void relaunch();
	bool isActive() const;
	unsigned int getTicks() const;
	void setTicks(unsigned int ticks);
	void adjust(unsigned int newMaximum);
};
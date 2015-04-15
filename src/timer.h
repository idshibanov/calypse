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
	unsigned getTicks() const;
	double getProgress() const;
	void setTicks(unsigned int ticks);
	void stop();
	void adjust(unsigned int newMaximum);
};
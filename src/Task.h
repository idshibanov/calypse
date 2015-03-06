#pragma once
#include "Point.h"
#include "Timer.h"
#include "Pathfinder.h"

class ActorTask {
public:
	ActorTask();
	~ActorTask();
	void setDestination(const Point&);
	void update();
	int getDirection(const Point&);
	void move(const Point&);
};
#pragma once
#include "Point.h"
#include "Object.h"
#include "Timer.h"

enum ActionType {
	ACTION_MOVE,
	ACTION_CUT,
	ACTION_END
};

class Action {
	TaskTimer _timer;
	
public:
	Action();
	~Action();
	void cut(weak_ptr<MapObject> target);
	void stop();
	bool update();
};
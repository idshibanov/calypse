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
	ActionType _type;
	TaskTimer _timer;
	Point _target;
public:
	Action(ActionType, int, int, const Point&, weak_ptr<MapObject>);
	Action(const Action&);
	~Action();
	Action& operator= (const Action&);
	void stop();
	bool isActive() const;
	bool update();
};
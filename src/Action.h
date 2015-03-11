#pragma once
#include "Point.h"
#include "Timer.h"
#include "Pathfinder.h"

class Actor;

enum ActionType {
	ACTION_MOVE,
	ACTION_CUT,
	ACTION_END
};

class Action {
	ActionType _type;
	weak_ptr<Actor> _actor;
	TaskTimer _timer;
	TaskTimer _state;
	Point _target;
	vector<AStarNode> _path;
	weak_ptr<AStarSearch> _pf;
public:
	Action(ActionType, weak_ptr<Actor>, int, int, const Point&, weak_ptr<AStarSearch>);
	Action(const Action&);
	~Action();
	Action& operator= (const Action&);
	void stop();
	bool isActive() const;
	int getState() const;
	bool update();
};
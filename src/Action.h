#pragma once
#include "Point.h"
#include "Timer.h"
#include "Pathfinder.h"

class MapObject;
class Actor;

enum ActionType {
	ACTION_MOVE,
	ACTION_CUT,
	ACTION_END
};

class Action {
	ActionType _type;
protected:
	weak_ptr<Actor> _actor;
	TaskTimer _timer;
	TaskTimer _state;
	Point _target;
	vector<AStarNode> _path;
	weak_ptr<AStarSearch> _pf;
	shared_ptr<Action> _next;
public:
	Action(ActionType, weak_ptr<Actor>, int, int, const Point&, weak_ptr<AStarSearch>);
	Action(const Action&);
	virtual ~Action();
	Action& operator= (const Action&);
	void chainAction(shared_ptr<Action>);
	shared_ptr<Action> getNext() const;
	void stop();
	bool isActive() const;
	int getState() const;
	virtual bool update();
};


class MoveAction : Action {
	Point _target;
	vector<AStarNode> _path;
	weak_ptr<AStarSearch> _pf;
public:
	MoveAction();
	MoveAction(const MoveAction&);
	~MoveAction();
	MoveAction& operator= (const MoveAction&);
	bool update();
};


class ObjectAction : Action {
	weak_ptr<MapObject> _t;
public:
	ObjectAction();
	ObjectAction(const ObjectAction&);
	~ObjectAction();
	ObjectAction& operator= (const ObjectAction&);
	bool update();

};
#pragma once
#include "Timer.h"
#include "Pathfinder.h"
#include "ResourceCtl.h"

class LocalMap;
class MapObject;
class Actor;

enum ActionType {
	ACTION_MOVE,
	ACTION_CUT,
	ACTION_DRAG,
	ACTION_DROP,
	ACTION_CRAFT,
	ACTION_END
};

class Action {
protected:
	weak_ptr<ResourceCtl> _res;
	ActionType _type;
	weak_ptr<Actor> _actor;
	TaskTimer _timer;
	TaskTimer _state;
	shared_ptr<Action> _next;
	Action(ActionType, weak_ptr<ResourceCtl>, weak_ptr<Actor>, int, int);
public:
	Action(const Action&);
	virtual ~Action();
	Action& operator= (const Action&);
	void chainAction(shared_ptr<Action>);
	shared_ptr<Action> getNext() const;
	void stop();
	int getState() const;
	ActionType getType() const;
	virtual bool isActive() const = 0;
	virtual void start() = 0;
	virtual bool update() = 0;
};


class MoveAction : public Action {
	Point _target;
	std::vector<AStarNode> _path;
	weak_ptr<AStarSearch> _pf;
public:
	MoveAction(ActionType, weak_ptr<ResourceCtl>, weak_ptr<Actor>, int, int, const Point&, weak_ptr<AStarSearch>);
	MoveAction(const MoveAction&);
	~MoveAction();
	MoveAction& operator= (const MoveAction&);
	bool isActive() const;
	void start();
	bool update();
};


class ObjectAction : public Action {
	weak_ptr<MapObject> _target;
	weak_ptr<LocalMap> _map;
	Point _targetPos;
public:
	ObjectAction(ActionType, weak_ptr<ResourceCtl>, weak_ptr<Actor>, int, int, weak_ptr<MapObject>, weak_ptr<LocalMap>);
	ObjectAction(const ObjectAction&);
	~ObjectAction();
	ObjectAction& operator= (const ObjectAction&);
	bool isActive() const;
	void start();
	bool update();
};


class PointAction : public Action {
	Point _targetPos;
	weak_ptr<LocalMap> _map;
public:
	PointAction(ActionType, weak_ptr<ResourceCtl>, weak_ptr<Actor>, int, int, const Point&, weak_ptr<LocalMap>);
	PointAction(const PointAction&);
	~PointAction();
	PointAction& operator= (const PointAction&);
	bool isActive() const;
	void start();
	bool update();
};
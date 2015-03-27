#pragma once
#include "Point.h"
#include "Timer.h"
#include "Pathfinder.h"

class LocalMap;
class MapObject;
class Actor;

enum ActionType {
	ACTION_MOVE,
	ACTION_CUT,
	ACTION_DRAG,
	ACTION_DROP,
	ACTION_END
};

class Action {
protected:
	ActionType _type;
	std::weak_ptr<Actor> _actor;
	TaskTimer _timer;
	TaskTimer _state;
	std::shared_ptr<Action> _next;
	Action(ActionType, std::weak_ptr<Actor>, int, int);
public:
	Action(const Action&);
	virtual ~Action();
	Action& operator= (const Action&);
	void chainAction(std::shared_ptr<Action>);
	std::shared_ptr<Action> getNext() const;
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
	std::weak_ptr<AStarSearch> _pf;
public:
	MoveAction(ActionType, std::weak_ptr<Actor>, int, int, const Point&, std::weak_ptr<AStarSearch>);
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
	ObjectAction(ActionType, std::weak_ptr<Actor>, int, int, std::weak_ptr<MapObject>, std::weak_ptr<LocalMap>);
	ObjectAction(const ObjectAction&);
	~ObjectAction();
	ObjectAction& operator= (const ObjectAction&);
	bool isActive() const;
	void start();
	bool update();
};


class PointAction : public Action {
	Point _targetPos;
public:
	PointAction(ActionType, std::weak_ptr<Actor>, int, int, const Point&);
	PointAction(const PointAction&);
	~PointAction();
	PointAction& operator= (const PointAction&);
	bool isActive() const;
	void start();
	bool update();
};
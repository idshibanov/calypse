#pragma once
#include "Timer.h"
#include "Pathfinder.h"
#include "ResourceCtl.h"
#include "ActionInfo.h"
#include "Item.h"

class LocalMap;
class MapObject;
class Actor;

class Action {
protected:
	shared_ptr<ActionInfo> _info;
	weak_ptr<ResourceCtl> _res;
	weak_ptr<Actor> _actor;
	TaskTimer _timer;
	TaskTimer _state;
	shared_ptr<Action> _next;
	Action(shared_ptr<ActionInfo>, weak_ptr<ResourceCtl>, weak_ptr<Actor>);
public:
	Action(const Action&);
	virtual ~Action();
	Action& operator= (const Action&);
	void chainAction(shared_ptr<Action>);
	shared_ptr<Action> getNext() const;
	void stop();
	int getState() const;
	shared_ptr<ActionInfo> getType() const;
	virtual bool isActive() const = 0;
	virtual void start() = 0;
	virtual bool update() = 0;
};


class MoveAction : public Action {
	Point _target;
	std::vector<AStarNode> _path;
	weak_ptr<AStarSearch> _pf;
public:
	MoveAction(shared_ptr<ActionInfo>, weak_ptr<ResourceCtl>, weak_ptr<Actor>, const Point&, weak_ptr<AStarSearch>);
	MoveAction(const MoveAction&);
	virtual ~MoveAction();
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
	ObjectAction(shared_ptr<ActionInfo>, weak_ptr<ResourceCtl>, weak_ptr<Actor>, weak_ptr<MapObject>, weak_ptr<LocalMap>);
	ObjectAction(const ObjectAction&);
	virtual ~ObjectAction();
	ObjectAction& operator= (const ObjectAction&);
	bool isActive() const;
	void start();
	bool update();
};


class PointAction : public Action {
	Point _targetPos;
	weak_ptr<LocalMap> _map;
public:
	PointAction(shared_ptr<ActionInfo>, weak_ptr<ResourceCtl>, weak_ptr<Actor>, const Point&, weak_ptr<LocalMap>);
	PointAction(const PointAction&);
	virtual ~PointAction();
	PointAction& operator= (const PointAction&);
	bool isActive() const;
	void start();
	bool update();
};


class ItemAction : public Action {
	shared_ptr<Item> _target;
	weak_ptr<LocalMap> _map;
public:
	ItemAction(shared_ptr<ActionInfo>, weak_ptr<ResourceCtl>, weak_ptr<Actor>, shared_ptr<Item>, weak_ptr<LocalMap>);
	ItemAction(const ItemAction&);
	virtual ~ItemAction();
	ItemAction& operator= (const ItemAction&);
	bool isActive() const;
	void start();
	bool update();
};
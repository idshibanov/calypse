#pragma once
#include <vector>
#include "Main.h"
#include "Point.h"
#include "Action.h"

class MapObject {
protected:
	Point _pos;
	short _type;
	int _spriteID;
	
	bool _dragged;
	Point* _masterPos;

	int _id;
	static int lastID;
public:
	MapObject(short type, const Point& pos);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
	virtual ~MapObject();
	int getType();
	int getID();
    int getXPos();
    int getYPos();
	Point getPos();
	void setPos(const Point&);
	int getSprite();
	void setSprite(int);
	void lift(Point* master);
	void dragged(bool);
	bool isDragged() const;
};

class Actor : public MapObject {
	bool _static;
	int _dir;
	int _timer;
	shared_ptr<Action> _action;
	weak_ptr<MapObject> _slave;
public:
	Actor(short type, const Point& pos, int defaultSprite);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
	void setAction(std::shared_ptr<Action>);
	void update();
	int getDirection(const Point&);
	void move(const Point&);
	void pickUp(weak_ptr<MapObject>);
	void drop(const Point&);
	bool isHolding() const;
	weak_ptr<MapObject> getSlave();
};

class SmallObject : public MapObject {
	Actor* _owner;
public:
	SmallObject(short, const Point&);
	~SmallObject();
	void pickUp(const Actor*);
};
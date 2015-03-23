#pragma once
#include <vector>
#include "Main.h"
#include "Point.h"
#include "Action.h"

class MapObject {
protected:
	Point _pos;
	short _type;
	bool _dragged;
	int _spriteID;
	int _id;
	static int lastID;
public:
    MapObject(const Point& pos, short type);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
    virtual ~MapObject();
    int getXPos();
    int getYPos();
	Point getPos();
	int getType();
	int getID();
	int getSprite();
	void setSprite(int);
};

class Actor : public MapObject {
	bool _static;
	int _dir;
	int _timer;
	std::shared_ptr<Action> _action;
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
};

class SmallObject : public MapObject {
	bool _dragged;
	Actor* _owner;
public:
	SmallObject(short, const Point&);
	~SmallObject();
	bool isDragged() const;
	void pickUp(const Actor*);
};
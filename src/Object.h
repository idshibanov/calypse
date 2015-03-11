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
	int _id;
	static int lastID;
public:
    MapObject(Point pos, short type);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
    ~MapObject();
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
	shared_ptr<Action> _action;
public:
	Actor(short type, Point pos, int defaultSprite);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
	void setAction(shared_ptr<Action>);
	void update();
	int getDirection(const Point&);
	void move(const Point&);
};
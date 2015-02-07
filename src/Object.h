#pragma once
#include <vector>
#include "Main.h"
#include "Point.h"
#include "Pathfinder.h"
#include "Timer.h"


class MapObject {
protected:
	Point _pos;
    short _type;
public:
    MapObject(Point pos, short type);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
    ~MapObject();
    int getXPos();
    int getYPos();
	Point getPos();
	int getType();
};

class Actor : public MapObject {
	bool _static;
	int _dir;
	Point _dest;
	int _spriteID;
	int _timer;
	weak_ptr<AStarSearch> _pathFinder;
	std::vector<AStarNode> _path;
public:
	Actor(short type, Point pos, int defaultSprite, weak_ptr<AStarSearch> pf);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
	int getSprite();
	void setDestination(const Point&);
	void update();
	int getDirection(const Point&);
	void move(const Point&);
};
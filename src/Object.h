#pragma once
#include <vector>
#include "Main.h"
#include "Point.h"
#include "Pathfinder.h"
#include "Timer.h"


class MapObject {
    short _type;
    bool _deleted;
protected:
	Point _pos;
	Point _size;
public:
    MapObject(Point pos, Point size, short type);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
    ~MapObject();
    int getXPos();
    int getYPos();
	Point getPos();
	Point getSize();
	int getType();
    bool isDeleted();
    void deleteObject();
};

class Actor : public MapObject {
	bool _static;
	int _dir;
	unsigned _destX;
	unsigned _destY;
	int _spriteID;
	int _timer;
	weak_ptr<AStarSearch> _pathFinder;
	std::vector<AStarNode> _path;
public:
	Actor(short type, Point pos, Point size, int defaultSprite, weak_ptr<AStarSearch> pf);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
	int getSprite();
	void setDestination(const Point&);
	void update();
	int getDirection(const Point&);
};
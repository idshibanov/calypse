#pragma once
#include <vector>
#include "Main.h"
#include "Pathfinder.h"
#include "Timer.h"


class MapObject {
    short _type;
    bool _deleted;
protected:
	int _xpos;
	int _ypos;
public:
    MapObject(int xpos, int ypos, short type);
    MapObject(MapObject& rhs);
    MapObject& operator=(MapObject& rhs);
    ~MapObject();
    int getXPos();
    int getYPos();
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
	Actor(int xpos, int ypos, short type, int defaultSprite, weak_ptr<AStarSearch> pf);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
	int getSprite();
	void setDestination(int, int);
	void update();
};
#pragma once
#include "main.h"

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
	int _default;
public:
	Actor(int xpos, int ypos, short type, int defaultSprite);
	Actor(Actor& rhs);
	Actor& operator=(Actor& rhs);
	~Actor();
	void setXPos(int);
	void setYPos(int);
};
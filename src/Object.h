#pragma once
#include "main.h"

class MapObject {
    int _xpos;
    int _ypos;
    short _type;
    bool _deleted;
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
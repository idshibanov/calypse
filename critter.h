#pragma once
#include "main.h"
#include "pathfinder.h"

class TCritter {
    unsigned _tileX;
    unsigned _tileY;
    unsigned _destX;
    unsigned _destY;
    int _timer;
    weak_ptr<AStarSearch> _pathFinder;
    std::vector<AStarNode> _path;
    int _spriteID;
public:
    TCritter(weak_ptr<AStarSearch>);
    TCritter(TCritter& rhs);
    TCritter& operator=(TCritter& rhs);
    ~TCritter();
    unsigned getXPos();
    unsigned getYPos();
    int getSpriteID();
    void setXPos(unsigned x);
    void setYPos(unsigned y);
    void setDest(unsigned x, unsigned y);
    void setSpriteID(int id);
    void update();
};
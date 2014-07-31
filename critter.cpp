#include "critter.h"

TCritter::TCritter(weak_ptr<AStarSearch> pFinder) {
    _tileX = 0;
    _tileY = 0;
    _destX = 0;
    _destY = 0;
    _timer = 0;
    _spriteID = 51;
    _pathFinder = pFinder;
}

TCritter::TCritter(TCritter& rhs) {
    *this = rhs;
}

TCritter& TCritter::operator=(TCritter& rhs) {
    _tileX = rhs._tileX;
    _tileY = rhs._tileY;
    _destX = rhs._destX;
    _destY = rhs._destY;
    _pathFinder = rhs._pathFinder;
    return *this;
}

TCritter::~TCritter() {

}

unsigned TCritter::getXPos() {
    return _tileX;
}

unsigned TCritter::getYPos() {
    return _tileY;
}

int TCritter::getSpriteID() {
    return _spriteID;
}

void TCritter::setXPos(unsigned x) {
    _tileX = x;
    _destX = x;
}

void TCritter::setYPos(unsigned y) {
    _tileY = y;
    _destY = y;
}

void TCritter::setDest(unsigned x, unsigned y) {
    _timer = 0;
    _destX = x;
    _destY = y;
    _path = _pathFinder.lock()->searchPath(_tileX, _tileY, _destX, _destY);
    for (auto it = _path.begin(); it != _path.end(); ++it) {
        std::cout << (*it) << endl;
    }
    std::cout << endl;
}

void TCritter::setSpriteID(int id) {
    _spriteID = id;
}

void TCritter::update() {
    if(!_path.empty()) {
        _timer++;
        if(_timer % 5 == 0) {
            AStarNode& nextNode = _path.front(); 
            _tileX = nextNode._mapX;
            _tileY = nextNode._mapY;
            _path.erase(_path.begin());
        }
    }
}

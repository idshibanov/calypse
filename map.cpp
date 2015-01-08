#include <cstdlib>
#include <ctime>
#include "map.h"
#include "critter.h"
#include "pathfinder.h"

GameMap::GameMap() {

}

GameMap::~GameMap(){

}

void GameMap::generate(weak_ptr<AStarSearch> pf) {
    _pFinder = pf;
    srand((unsigned)time(NULL));
    _rowmax = TD_MAP_ROWS;
    _colmax = TD_MAP_COLS;
    for(unsigned row = 0; row < _rowmax; row++){
        for(unsigned col = 0; col < _colmax; col++){
            _tiles.push_back(MapTile(8 + (rand() % 6)));
        }
    }

    unsigned pRow = 0, pCol = 0;
    pRow += rand() % 7;
    while(pRow < _rowmax){
        pCol += rand() % 15;
        while(pCol < _colmax){
            _tiles[pRow * _colmax + pCol].setType(47);
            pCol += rand() % 15;
        } 
        pRow += rand() % 7;
        pCol = 0;
    }

    // tree generation (sprite 79)
    int objMaxDensity = 10; // 10% max
    int objPos = 0;
    int tempX = 0;
    int tempY = 0;

    for(unsigned row = 0; row < _rowmax; row += 10){
        for(unsigned col = 0; col < _colmax; col += 10){
            for(int k = 0; k < objMaxDensity; k++){
                objPos = rand() % 100;
                tempX = col+(objPos % 10);
                tempY = row+(objPos/10);
                shared_ptr<GameObject> tempObj(new GameObject(tempX, tempY, 79));
                _objects.push_back(tempObj);
                _tiles[tempY * _colmax + tempX].setObject(tempObj);
            }
        }
    }

    // Player - temporary
    _critter.push_back(make_shared<TCritter>(_pFinder));
}

short GameMap::getTileType(unsigned x, unsigned y) const { 
    return getTileType(y*_colmax+x); 
}

short GameMap::getTileType(unsigned id) const { 
    return _tiles[id].getType(); 
}

unsigned int GameMap::getRowMax() const { 
    return _rowmax; 
}

unsigned int GameMap::getColMax() const { 
    return _colmax; 
}

unsigned int GameMap::convertIDToX(unsigned mapID) const {
    return mapID % _colmax;
}

unsigned int GameMap::convertIDToY(unsigned mapID) const {
    return mapID / _colmax;
}

bool GameMap::tileExists(unsigned mapID) const {
    return mapID < (_rowmax * _colmax);
}

bool GameMap::tileExists(unsigned tileX, unsigned tileY) const {
    return tileY < _rowmax && tileX < _colmax;
}

bool GameMap::tileIsFree(unsigned mapID) const {
    bool retval = false;
    if(tileExists(mapID)) {
        if(_tiles[mapID].getObject().expired())
            retval = true;
    }
    return retval;
}

bool GameMap::tileIsFree(unsigned tileX, unsigned tileY) const {
    bool retval = false;
    if(tileX < _colmax && tileY < _rowmax)
        retval = tileIsFree(tileY * _colmax + tileX);
    return retval;
}

weak_ptr<GameObject> GameMap::getObject(unsigned tileX, unsigned tileY) const{
    return _tiles[tileY * _colmax + tileX].getObject();
}

weak_ptr<TCritter> GameMap::getCritter() const {
    return _critter[0];
}

const vector<shared_ptr<GameObject>>& GameMap::getObjectVector() const{
    return _objects;
}

void GameMap::setTile(unsigned id, unsigned value) { 
    _tiles[id].setType(value); 
}

void GameMap::processAction(int x, int y, int id) {
    weak_ptr<GameObject> obj = _tiles[y * _colmax + x].getObject();
    
    if(id == 2 && !obj.expired() && !obj.lock()->isDeleted()) {
        obj.lock()->deleteObject();
        cout << "Removed obj: " << obj.lock()->isDeleted() << endl;
        _tiles[y * _colmax + x].resetObject();
    } else if (obj.expired() && id == 2) {
        shared_ptr<GameObject> tempObj(new GameObject(x, y, 79));
        _objects.push_back(tempObj);
        _tiles[y * _colmax + x].setObject(tempObj);
        cout << "Added obj: " << tempObj->isDeleted() << endl;
    } else if (id == 3) {
        if (!obj.expired()) {
            cout << "Found obj: deleted is " << obj.lock()->isDeleted() << endl;
        } else {
            cout << "No obj found!" << endl;
            for (auto it = _objects.begin(); it != _objects.end() ; ++it){
                if ( (*it)->getXPos() == x && (*it)->getYPos() == y ) {
                    cout << "Object in the array: " << (*it)->isDeleted() << endl;
                }
            }
        }
    }
}

void GameMap::update() {
    _critter[0]->update();
}
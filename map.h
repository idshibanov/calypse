#pragma once
#include <vector>
#include <bitset>
#include "main.h"
#include "object.h"
#include "tile.h"

class AStarSearch;
class TCritter;

class GameMap {
    weak_ptr<AStarSearch> _pFinder;
    vector<MapTile> _tiles;
    vector<shared_ptr<GameObject>> _objects;
    vector<shared_ptr<TCritter>> _critter;
    unsigned int _rowmax;
    unsigned int _colmax;
public:
    GameMap();
    ~GameMap();
    void generate(weak_ptr<AStarSearch> pf);
    short getTileType(unsigned x, unsigned y) const;
    short getTileType(unsigned id) const;
    unsigned int getRowMax() const;
    unsigned int getColMax() const;
    unsigned int convertIDToX(unsigned mapID) const;
    unsigned int convertIDToY(unsigned mapID) const;
    bool tileExists(unsigned mapID) const;
    bool tileExists(unsigned tileX, unsigned tileY) const;
    bool tileIsFree(unsigned mapID) const;
    bool tileIsFree(unsigned tileX, unsigned tileY) const;
    weak_ptr<GameObject> getObject(unsigned tileX, unsigned tileY) const;
    weak_ptr<TCritter> getCritter() const;
    const vector<shared_ptr<GameObject>>& getObjectVector() const;
    void setTile(unsigned id, unsigned value);
    void processAction(int x, int y, int id);
    void update();
};
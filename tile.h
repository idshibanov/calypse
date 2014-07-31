#pragma once
#include "main.h"
#include "object.h"

class MapTile {
    short _type;
    weak_ptr<GameObject> _obj;
public:
    MapTile(short type);
    MapTile(short type, const shared_ptr<GameObject>& obj);
    ~MapTile();
    short getType() const;
    weak_ptr<GameObject> getObject() const;
    void setType(short type);
    void setObject(weak_ptr<GameObject> obj);
    void resetObject();
};
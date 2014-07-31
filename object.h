#pragma once
#include "main.h"

class GameObject {
    int _xpos;
    int _ypos;
    short _type;
    bool _deleted;
public:
    GameObject(int xpos, int ypos, short type);
    GameObject(GameObject& rhs);
    GameObject& operator=(GameObject& rhs);
    ~GameObject();
    int getXPos();
    int getYPos();
    int getType();
    bool isDeleted();
    void deleteObject();
};
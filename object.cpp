#include "object.h"

GameObject::GameObject(int xpos, int ypos, short type) 
                     : _xpos(xpos), _ypos(ypos), _type(type) {
    _deleted = false;
}

GameObject::GameObject(GameObject& rhs){
    *this = rhs;
}

GameObject& GameObject::operator=(GameObject& rhs){
    _xpos = rhs._xpos;
    _ypos = rhs._ypos;
    _type = rhs._type;
    _deleted = rhs._deleted;
    return *this;
}

GameObject::~GameObject(){

}

int GameObject::getXPos(){
    return _xpos;
}

int GameObject::getYPos(){
    return _ypos;
}

int GameObject::getType(){
    return _type;
}

bool GameObject::isDeleted(){
    return _deleted;
}

void GameObject::deleteObject(){
    _deleted = true;
}

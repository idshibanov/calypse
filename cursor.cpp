#include "cursor.h"

TCursor::TCursor(unsigned x, unsigned y){
    _xpos = x;
    _ypos = y;
}

TCursor::~TCursor(){

}

unsigned TCursor::getXPos(){
    return _xpos;
}

unsigned TCursor::getYPos(){
    return _ypos;
}

void TCursor::setPos(unsigned x, unsigned y){
    _xpos = x;
    _ypos = y;
}

void TCursor::decreaseX(){
    _xpos--;
}

void TCursor::decreaseY(){
    _ypos--;
}

void TCursor::increaseX(){
    _xpos++;
}

void TCursor::increaseY(){
    _ypos++;
}
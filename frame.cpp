#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "frame.h"
#include "element.h"

TFrame::TFrame(){
    _type = TFRAME_TYPE_INVALID;
    _xpos = 0;
    _ypos = 0;
    _xsize = 0;
    _ysize = 0;
    _zlevel = 0;
    _draggable = false;
    _parent = 0;
}

TFrame::TFrame(TFrameType type, int xpos, int ypos, unsigned xsize, unsigned ysize, 
               int zlevel, bool draggable, bool visible, TFrame* parent){
        _type = type;
        _xpos = xpos;
        _ypos = ypos;
        _xsize = xsize;
        _ysize = ysize;
        _zlevel = zlevel;
        _draggable = draggable;
        _visible = visible;
        _parent = parent;
}

TFrame::~TFrame() {

}

int TFrame::getXPos() {
    return _xpos; 
}

int TFrame::getYPos() { 
    return _ypos; 
}

int TFrame::getXMaxPos() {
    return _xpos + _xsize; 
}

int TFrame::getYMaxPos() { 
    return _ypos + _ysize; 
}

int TFrame::getZLevel() { 
    return _zlevel; 
}

TFrameType TFrame::getType() {
    return _type;
}

int TFrame::getAbsXPos() {
    int retval = _xpos;
    if(_parent != NULL)
        retval += _parent->getAbsXPos();
    return retval; 
}

int TFrame::getAbsYPos() {
    int retval = _ypos;
    if(_parent != NULL)
        retval += _parent->getAbsYPos();
    return retval; 
}

bool TFrame::isVisible() {
    return _visible;
}

void TFrame::move(int xpos, int ypos) { 
    _xpos = xpos; 
    _ypos = ypos; 
}

void TFrame::addElement(shared_ptr<UIElement> elem){
    if (elem != NULL)
        _elem.push_back(elem);
}

shared_ptr<UIElement> TFrame::getElement(int absX, int absY){
    shared_ptr<UIElement> retval = NULL;
    bool found = false;
    int frame_X = getAbsXPos();
    int frame_Y = getAbsYPos();
    for (auto it = _elem.begin(); !found && it != _elem.end() ; ++it){
        int x1 = (*it)->getXPos();
        int y1 = (*it)->getYPos();
        int x2 = (*it)->getXMax();
        int y2 = (*it)->getYMax();
        if(absX >= (frame_X + x1) && absY >= (frame_Y + y1)){
            if(absX <= (frame_X + x2) && absY <= (frame_Y + y2)){
                found = true;
                retval = *it;
            }
        }
    }
    return retval;
}

void TFrame::setParent(TFrame* p) { 
    _parent = p; 
}

void TFrame::setZLevel(unsigned z) { 
    _zlevel = z; 
}

void TFrame::setVisible(bool value) {
    _visible = value;
}

void TFrame::setSprite(shared_ptr<Sprite> sprite) {
    _bgSprite = sprite;
}

void TFrame::draw() {
    if(_visible){
        if(_bgSprite == NULL){
            al_draw_filled_rectangle(_xpos, _ypos, _xpos + _xsize, _ypos+_ysize, al_map_rgb(230, 190, 150));
        } else {
            _bgSprite->drawScaled(_xpos, _ypos, _xsize, _ysize);
        }
    
        for (auto it = _elem.begin(); it != _elem.end() ; ++it){
            (*it)->draw();
        }
    }
}
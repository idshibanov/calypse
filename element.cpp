#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include "element.h"

// Abstract UI Element class
UIElement::UIElement(int xpos, int ypos, int xsize, int ysize, 
                    TFrame* parent, bool active, bool visible){
    _xpos = xpos;
    _ypos = ypos;
    _xsize = xsize;
    _ysize = ysize;
    _parent = parent;
    _active = active;
    _visible = visible;
    _type = UIELEMENT_TYPE_INVALID;
}

UIElement::~UIElement() {

}

int UIElement::getXPos(){
    return _xpos;
}

int UIElement::getYPos(){
    return _ypos;
}

int UIElement::getXMax(){
    return _xpos + _xsize;
}

int UIElement::getYMax(){
    return _ypos + _ysize;
}

int UIElement::getType(){
    return _type;
}

bool UIElement::isActive(){
    return _active;
}

bool UIElement::isVisible(){
    return _visible;
}

// Label class
UILabel::UILabel(int xpos, int ypos, int xsize, int ysize, TFrame* parent, 
                SpriteText* font, string& text, bool active, bool visible) 
                : UIElement(xpos, ypos, xsize, ysize, parent, active, visible){
    _text = text;
    _font = font;
    _type = UIELEMENT_TYPE_LABEL;
}

UILabel::~UILabel(){

}

string& UILabel::getText(){
    return _text;
}

void UILabel::update(){

}

void UILabel::draw(){
    int drawX = _xpos;
    int drawY = _ypos;
    if(_parent){
        drawX += _parent->getAbsXPos();
        drawY += _parent->getAbsYPos();
    }

    // add text positioning
    if (!_text.empty())
        _font->draw(_text, drawX, drawY, al_map_rgb(20, 20, 20));
}


// Button class
UIButton::UIButton(int xpos, int ypos, TFrame* parent, int xsize, int ysize, int action_id, 
                shared_ptr<Sprite> spr, shared_ptr<Sprite> sprOn, SpriteText* font, string& text, 
                bool active, bool visible, bool state, unsigned clickedTicks) 
                : UIElement(xpos, ypos, xsize, ysize, parent, active, visible), _clickTimer(clickedTicks){
    _action_id = action_id;
    _sprite = spr;
    _spriteClicked = sprOn;
    _text = text;
    _state = state;
    _font = font;
    _type = UIELEMENT_TYPE_BUTTON;
}

UIButton::~UIButton(){

}

int UIButton::getActionID(){
    return _action_id;
}

bool UIButton::getState(){
    return _state;
}

string& UIButton::getText(){
    return _text;
}

void UIButton::launchTimer(){
    _clickTimer.relaunch();
}

bool UIButton::checkTimer(){
    if(_clickTimer.isActive() == false)
        _clickTimer.relaunch();

    return _clickTimer.check();
}

void UIButton::update(){
    _clickTimer.check();
}

void UIButton::draw(){
    int drawX = _xpos;
    int drawY = _ypos;
    if(_parent){
        drawX += _parent->getAbsXPos();
        drawY += _parent->getAbsYPos();
    }

    if(_sprite){
        if(_clickTimer.isActive() && _spriteClicked){
            _spriteClicked->draw(drawX, drawY);
        } else {
            _sprite->draw(drawX, drawY);
        }
    } else {
        al_draw_filled_rectangle(drawX, drawY, drawX + _xsize, drawY+_ysize, al_map_rgb(230, 30, 30));
    }

    // add text positioning
    if (!_text.empty())
        _font->draw(_text, drawX, drawY, al_map_rgb(20, 20, 20));
}
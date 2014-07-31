#pragma once
#include <string>
#include "main.h"
#include "frame.h"
#include "sprite.h"
#include "timer.h"

#define UIBUTTON_DEFAULT_CLICKED_TICKS 10

enum UIElementType {
    UIELEMENT_TYPE_BUTTON,
    UIELEMENT_TYPE_LABEL,
    UIELEMENT_TYPE_INVALID
};

class UIElement {
protected:
    int _xpos;
    int _ypos;
    int _xsize;
    int _ysize;
    TFrame* _parent;
    bool _active;
    bool _visible;
    UIElementType _type;
    UIElement(int xpos, int ypos, int xsize, int ysize, 
        TFrame* parent, bool active = true, bool visible = true);
public:
    virtual ~UIElement();
    int getXPos();
    int getYPos();
    int getXMax();
    int getYMax();
    int getType();
    bool isActive();
    bool isVisible();
    virtual void update() = 0;
    virtual void draw() = 0;
};

class UILabel : public UIElement {
    string _text;
    SpriteText* _font;
public:
    UILabel(int xpos, int ypos, int xsize, int ysize, TFrame* parent, 
            SpriteText* font, string& text, bool active = true, bool visible = true );
    ~UILabel();
    string& getText();
    void update();
    void draw();
};

class UIButton : public UIElement {
    int _action_id;
    bool _state;
    shared_ptr<Sprite> _sprite;
    shared_ptr<Sprite> _spriteClicked;
    TTimer _clickTimer;
    string _text;
    SpriteText* _font;
public:
    UIButton(int xpos, int ypos, TFrame* parent, int xsize, int ysize, int action_id,  
             shared_ptr<Sprite> spr, shared_ptr<Sprite> sprOn, SpriteText* font, string& text, 
             bool active = true, bool visible = true, bool state = false, 
             unsigned clickedTicks = UIBUTTON_DEFAULT_CLICKED_TICKS);
    ~UIButton();
    int getActionID();
    bool getState();
    string& getText();
    void launchTimer();
    bool checkTimer();
    void update();
    void draw();
};
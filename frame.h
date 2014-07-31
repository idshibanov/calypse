#pragma once
#include <vector>
#include "main.h"
#include "sprite.h"

class UIElement;

enum TFrameType {
    TFRAME_TYPE_VIEWPORT,
    TFRAME_TYPE_MENU,
    TFRAME_TYPE_INVALID
};

class TFrame {
    TFrameType _type;
    unsigned _xsize;
    unsigned _ysize;
    int _xpos;
    int _ypos;
    int _zlevel;
    bool _draggable;
    bool _visible;
    TFrame* _parent;

    vector<shared_ptr<UIElement>> _elem;
    shared_ptr<Sprite> _bgSprite;
    ALLEGRO_COLOR _bgColor;
public:
    TFrame();
    TFrame(TFrameType type, int xpos, int ypos, unsigned xsize, unsigned ysize,
           int zlevel = 0, bool draggable = false, bool visible = true, TFrame* parent = 0);
    virtual ~TFrame();
    int getXPos();
    int getYPos();
    int getXMaxPos();
    int getYMaxPos();
    int getZLevel();
    TFrameType getType();
    int getAbsXPos();
    int getAbsYPos();
    bool isVisible();
    void move(int xpos, int ypos);
    void addElement(shared_ptr<UIElement> elem);
    shared_ptr<UIElement> getElement(int absX, int absY);
    void setParent(TFrame* p);
    void setZLevel(unsigned z);
    void setVisible(bool value);
    void setSprite(shared_ptr<Sprite> sprite);
    void draw();
};
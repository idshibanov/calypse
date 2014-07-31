#pragma once
#include <vector>
#include <list>
#include "camera.h"
#include "cursor.h"
#include "element.h"
#include "frame.h"
#include "main.h"
#include "map.h"
#include "mouse.h"
#include "sprite.h"

// has to be float to work with integers
#define MAPTILE_SUBDIVISIONS 5.0

class ScreenCtl {
    ALLEGRO_DISPLAY* _display;
    unsigned _screenWidth;
    unsigned _screenHeight;
    int _tileWidth;
    int _tileHeight;
    
    shared_ptr<GameMap> _map;
    TCamera* _cam;
    TCursor* _cursor;
    TMouse* _mouse;

    vector<string> _strings;
    vector<TFrame> _frames;
    vector<shared_ptr<UIElement>> _elements;
    vector<shared_ptr<Sprite>> _sprites;
    vector<SpriteSheet> _tilesets;
    vector<SpriteText> _fonts;
    
    unsigned _tileCol;
    unsigned _tileRow;
    int _renderX; 
    int _renderY;
    float _zoom;

    ScreenCtl(const ScreenCtl&);
    ScreenCtl& operator=(const ScreenCtl&);
public:
    ScreenCtl(shared_ptr<GameMap> map, TCamera* cam, TCursor* cursor);
    ~ScreenCtl();
    void resizeScreen(unsigned width, unsigned height);
    bool isGameFrame(int absX, int absY);
    float convertScreenX(int absX);
    float convertScreenY(int absY);
    TFrame* getFrame(int absX, int absY);
    shared_ptr<UIElement> getElement(int absX, int absY);
    int getAction(int absX, int absY);
    int processUI(int absX, int absY);
    int getRenderScreenX();
    int getRenderScreenY();
    int getTileCol();
    int getTileRow();
    void zoomIn();
    void zoomOut();
    void processAction(int x, int y, int button);
    void update();
    void draw(TMouse& mouse , int gameFPS);
};
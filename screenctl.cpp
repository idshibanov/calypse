#include <allegro5/allegro5.h>
#include <string>
#include "screenctl.h"
#include "element.h"
#include "critter.h"
#include "object.h"

ScreenCtl::ScreenCtl(shared_ptr<GameMap> map, TCamera* cam, TCursor* cursor){
    _screenWidth = TD_DISPLAY_WIDTH;
    _screenHeight = TD_DISPLAY_HEIGHT;
    _tileWidth = TD_TILESIZE_X;
    _tileHeight = TD_TILESIZE_Y;

    _display = al_create_display(_screenWidth, _screenHeight);
    al_hide_mouse_cursor(_display);

    _map = map;
    _cam = cam;
    _cursor = cursor;
    _renderX = 0; 
    _renderY = 0;
    _tileCol = 0;
    _tileRow = 0;
    _zoom = 1.0;

    // test elements:
    _strings.push_back("OK");
    _strings.push_back("Cancel");
    _strings.push_back("Action ID: ");

    _fonts.push_back(SpriteText("arialbd.ttf", 12));
    _fonts.push_back(SpriteText("arialbd.ttf", 16));
    
    _frames.push_back(TFrame(TFRAME_TYPE_VIEWPORT, 0, 0, _screenWidth, _screenHeight, 0));
    _frames.push_back(TFrame(TFRAME_TYPE_MENU, 100, 590, 102, 50, 1, false, true));

    _tilesets.push_back(SpriteSheet(0, "dustvg.png", 207, 16));
    _tilesets.push_back(SpriteSheet(1, "mack.png", 255, 16));

    _sprites.push_back(make_shared<Sprite>(0, "cursor.png"));
    _sprites.push_back(make_shared<Sprite>(1, "back.png"));
    _sprites.push_back(make_shared<Sprite>(2, "buttonR32.png"));
    _sprites.push_back(make_shared<Sprite>(3, "buttonG32.png"));
    
    _elements.push_back(make_shared<UIButton>(10, 10, &_frames[1], 32, 32, 1, _sprites[2], _sprites[3], &_fonts[1], string()));
    _elements.push_back(make_shared<UIButton>(60, 10, &_frames[1], 32, 32, 2, _sprites[2], _sprites[3], &_fonts[1], string()));

    _frames[1].addElement(_elements[0]);
    _frames[1].addElement(_elements[1]);
    _frames[1].setSprite(_sprites[1]);
}

ScreenCtl::~ScreenCtl(){
    al_destroy_display(_display);
}

void ScreenCtl::resizeScreen(unsigned width, unsigned height) {
    _screenWidth = width;
    _screenHeight = height;
    al_resize_display(_display, _screenWidth, _screenHeight);
}

bool ScreenCtl::isGameFrame(int absX, int absY){
    bool retval = false;
    TFrame* fr = getFrame(absX,absY);
    if(fr && fr->getType() == TFRAME_TYPE_VIEWPORT)
        retval = true;
    return retval;
}

float ScreenCtl::convertScreenX(int absX){
    // cast tileWidth to float to keep fraction part
    float retval = ((absX - _renderX) / (float)_tileWidth) + _tileCol;

    // To get .2 tile steps
    retval = floor(retval*5 + 0.5)/5;

    return retval;
}

float ScreenCtl::convertScreenY(int absY){
    // cast tileHeight to float to keep fraction part
    float retval = ((absY - _renderY) / (float)_tileHeight) + _tileRow;

    // To get .2 tile steps
    retval = floor(retval*5 + 0.5)/5;

    return retval;
}

TFrame* ScreenCtl::getFrame(int absX, int absY){
    TFrame* retval = 0;
    
    int maxzlevel = 0;
    for (vector<TFrame>::iterator it = _frames.begin(); it != _frames.end() ; ++it){
        if ( (*it).isVisible() ) {
            if (absX >= (*it).getAbsXPos() && absY >= (*it).getAbsYPos() 
                && absX <= (*it).getXMaxPos() && absY <= (*it).getYMaxPos() ) {

                if ( (*it).getZLevel() >= maxzlevel ) {
                    maxzlevel = (*it).getZLevel();
                    retval = &*it;
                }
            }
        }
    }

    return retval;
}

shared_ptr<UIElement> ScreenCtl::getElement(int absX, int absY){
    shared_ptr<UIElement> retval = 0;
    TFrame* fr = getFrame(absX,absY);
    if(fr)
        retval = fr->getElement(absX,absY);

    return retval;
}

int ScreenCtl::getAction(int absX, int absY){
    int retval = -1;
    shared_ptr<UIElement> elem = getElement(absX,absY);
    if(elem && elem->getType() == UIELEMENT_TYPE_BUTTON) {
        UIButton* butt = (UIButton*) elem.get();
        retval = butt->getActionID();
    }
    return retval;
}

int ScreenCtl::processUI(int absX, int absY){
    int retval = -1;
    shared_ptr<UIElement> elem = getElement(absX,absY);
    if(elem && elem->getType() == UIELEMENT_TYPE_BUTTON) {
        auto butt = dynamic_pointer_cast<UIButton>(elem); 

        butt->launchTimer();

        retval = butt->getActionID();
    }
    return retval;
}

int ScreenCtl::getRenderScreenX(){
    return _renderX;
}

int ScreenCtl::getRenderScreenY(){
    return _renderY;
}

int ScreenCtl::getTileCol(){
    return _tileCol;
}

int ScreenCtl::getTileRow(){
    return _tileRow;
}

void ScreenCtl::zoomIn() {
    if(_zoom < 2.0) {
        _zoom += 0.1;
        _tileWidth = TD_TILESIZE_X * _zoom;
        _tileHeight = TD_TILESIZE_Y * _zoom;
    }
}

void ScreenCtl::zoomOut() {
    if(_zoom > 0.5) {
        _zoom -= 0.1;
        _tileWidth = TD_TILESIZE_X * _zoom;
        _tileHeight = TD_TILESIZE_Y * _zoom;
    }
}

void ScreenCtl::processAction(int x, int y, int button){
    TFrame* fr = getFrame(x, y);
    if (fr && fr->getType() == TFRAME_TYPE_VIEWPORT) {
        int tileX = convertScreenX(x);
        int tileY = convertScreenY(y);
        cout << "Mouse click (" << button << "): " << tileX << ", " << tileY << endl;
        _map->processAction(tileX,tileY,button);
        //_cursor->setPos(tileX, tileY);
    }
}

void ScreenCtl::update(){
    for (auto it = _elements.begin(); it != _elements.end() ; ++it){
        (*it)->update();
    }
}

void ScreenCtl::draw(TMouse& mouse, int gameFPS){
    // map size
    unsigned rowmax = _map->getRowMax();
    unsigned colmax = _map->getColMax();
    // camera position
    unsigned camX = _cam->getXPos();
    unsigned camY = _cam->getYPos();
    // first tile to render - coords and array id
    int firstTileX = 0, firstTileY = 0;
    unsigned maxTileCol = 0, maxTileRow = 0;
    _tileCol = 0;
    _tileRow = 0;
    // display coords to start rendering
    _renderX = 0;
    _renderY = 0;
    

    if(camX < _screenWidth / 2){
        // adjust render X position (left border)
        _renderX = _screenWidth / 2 - camX;
    } else {
        // find out first tile to render
        firstTileX = camX - _screenWidth / 2;
        _tileCol = firstTileX / _tileWidth;

        // adjust render X position if cam X coord is uneven
        if(firstTileX % _tileWidth){
            _renderX = -(firstTileX % _tileWidth);
        }
    }

    if(camY < _screenHeight / 2){
        // adjust render Y position (top border)
        _renderY = _screenHeight / 2 - camY;
    } else {
        // find out first tile to render
        firstTileY = camY - _screenHeight / 2;
        _tileRow = firstTileY / _tileHeight;

        // adjust render Y position if cam Y coord is uneven
        if(firstTileY % _tileHeight){
            _renderY = -(firstTileY % _tileHeight);
        }
    }

    // +3 tiles to handle the camera movement
    maxTileCol = _tileCol + ((_renderX + _screenWidth) / _tileWidth) +3;
    if (colmax < maxTileCol) maxTileCol = colmax;
    maxTileRow = _tileRow + ((_renderY + TD_DISPLAY_HEIGHT) / _tileHeight) +3;
    if (rowmax < maxTileRow) maxTileRow = rowmax;
    
    // Map tiles
    for(unsigned row = _tileRow; row < maxTileRow; row++){
        for(unsigned col = _tileCol; col < maxTileCol; col++){
            _tilesets[0].drawScaled(_renderX + ((col - _tileCol) * _tileWidth), 
                              _renderY + ((row - _tileRow) * _tileHeight), 
                              _map->getTileType(row*colmax+col), _zoom );

            weak_ptr<GameObject> objRef = _map->getObject(col, row);
            if(!objRef.expired() && !objRef.lock()->isDeleted()){
                _tilesets[1].drawScaled(_renderX + ((col - _tileCol) * _tileWidth), 
                                  _renderY + ((row - _tileRow) * _tileHeight), 
                                  66, _zoom );//objRef.lock()->getType() );
            }
        }
    }

    
    weak_ptr<TCritter> crit = _map->getCritter();
    if (!crit.expired()){
        _tilesets[0].drawScaled(_renderX + ((crit.lock()->getXPos() - _tileCol) * _tileWidth), 
                          _renderY + ((crit.lock()->getYPos() - _tileRow) * _tileHeight), 
                          crit.lock()->getSpriteID(), _zoom );
    }
    

    // Camera
    //_tilesets[0].draw(TD_DISPLAY_WIDTH / 2, TD_DISPLAY_HEIGHT / 2, 51);

    // Cursor
    /*
    if(_cursor->getXPos() >= _tileCol && _cursor->getYPos() >= _tileRow && 
       _cursor->getXPos() <= maxTileCol && _cursor->getYPos() <= maxTileRow){
        _tilesets[0].draw(_renderX + ((_cursor->getXPos() - _tileCol) * _tileWidth), 
                          _renderY + ((_cursor->getYPos() - _tileRow) * _tileHeight), 52 );
    }
    */

    // UI
    _frames[1].draw();

    string fpsSTR("FPS: " + to_string(static_cast<long long>(gameFPS)) + " Zoom: " + to_string(static_cast<long double>(_zoom)));
    _fonts[0].draw(fpsSTR, 5,5, al_map_rgb(255, 255, 255));

    // Almost last: mouse cursor
    _sprites[0]->draw(mouse.getXPos(), mouse.getYPos());


}
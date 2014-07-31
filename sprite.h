#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "main.h"

class Sprite {
protected:
    unsigned int _id;
	ALLEGRO_BITMAP* _texture;
public:
    Sprite(unsigned int, const char* filename);
    virtual ~Sprite();
    Sprite(const Sprite&);
    Sprite& operator= (const Sprite&);
    void draw(int x, int y);
    void drawScaled(int x, int y, int width, int height);
};

class SpriteSheet : public Sprite {
    unsigned int _last;
    unsigned int _xsize;
    unsigned int _ysize;
    unsigned int _rowsize;
public:
    SpriteSheet(unsigned id, const char* filename, unsigned last, 
                unsigned rowsize, unsigned xsize = 32, unsigned ysize = 32 );
    ~SpriteSheet() { }
    SpriteSheet(const SpriteSheet&);
    SpriteSheet& operator= (const SpriteSheet&);
    unsigned int getLastID() { return _last; };
    unsigned int getSpriteXSize() { return _xsize; };
    unsigned int getSpriteYSize() { return _xsize; };
    void draw(int x, int y, unsigned int sprite_id);
    void drawScaled(int x, int y, unsigned int sprite_id, float zoom);
    void drawRandom(int x, int y);
};

class SpriteText {
    ALLEGRO_FONT* _font;
    string _filename;
    int _fontsize;
public:
    SpriteText(const char* fontname, unsigned size);
    ~SpriteText();
    SpriteText(const SpriteText&);
    SpriteText& operator= (const SpriteText&);
    void draw(const char* str, int x, int y, ALLEGRO_COLOR color);
    void draw(string& str, int x, int y, ALLEGRO_COLOR color);
};
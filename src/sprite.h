#pragma once

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <string>
#include "main.h"
#include "Point.h"

class Sprite {
protected:
	unsigned int _id;
	ALLEGRO_BITMAP* _texture;
public:
	Sprite(unsigned int, const char* filename);
	virtual ~Sprite();
	Sprite(const Sprite&);
	Sprite& operator= (const Sprite&);
	void draw(const Point& pos);
	void drawScaled(const Point& pos, const Point& size);
	// TODO: Apply on other classes
	bool checkAlpha(const Point& pos);
};

class SpriteSheet : public Sprite {
	Point _size;
	unsigned int _last;
	unsigned int _rowsize;
public:
	SpriteSheet(unsigned id, const char* filename, unsigned last,
		unsigned rowsize, const Point& pos);
	~SpriteSheet() { }
	SpriteSheet(const SpriteSheet&);
	SpriteSheet& operator= (const SpriteSheet&);
	unsigned int getLastID() { return _last; };
	Point getSpriteSize() { return _size; };
	void draw(const Point& pos, unsigned int sprite_id);
	void drawScaled(const Point& pos, unsigned int sprite_id, float zoom);
	void drawRandom(const Point& pos);
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
	void draw(const char* str, const Point& pos, ALLEGRO_COLOR color);
	void draw(string& str, const Point& pos, ALLEGRO_COLOR color);
	void draw(const Point& text, const Point& pos, ALLEGRO_COLOR color);
};

class RawSprite : public Sprite {
	Point _size;
	Point _pos;
public:
	RawSprite(unsigned id, const char* filename, const Point& pos, const Point& size);
	~RawSprite() { }
	RawSprite(const RawSprite&);
	RawSprite& operator= (const RawSprite&);
	Point getSpriteSize() { return _size; };
	void draw(const Point& pos);
	void drawScaled(const Point& pos, float zoom);
};
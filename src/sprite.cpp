#include <allegro5\allegro_primitives.h>
#include <cstdlib>
#include "sprite.h"

//Sprite class
Sprite::Sprite(unsigned int id, const char* filename){
	_id = id;
	_texture = al_load_bitmap(filename);
	if (_texture == NULL){
		cout << "Warning: sprite " << _id << " is not loaded! (" << filename << ")" << endl;
	}
	else {
		//al_convert_mask_to_alpha(_texture, al_map_rgb(255, 0, 255));
		al_convert_mask_to_alpha(_texture, al_map_rgb(11, 0, 11));

		int sz = al_get_bitmap_height(_texture) * al_get_bitmap_width(_texture);
		cout << filename << " Bitmap format: " << al_get_bitmap_format(_texture) << " Size: " << sz << endl;
		//  70, 107, 107
	}
}

Sprite::Sprite(const Sprite& rhs) {
	_id = rhs._id;
	if (rhs._texture != NULL) {
		_texture = al_clone_bitmap(rhs._texture);
		if (_texture == NULL){
			cout << "Warning: sprite " << _id << " is not loaded! (clone error)" << endl;
		}
		else {
			al_convert_mask_to_alpha(_texture, al_map_rgb(255, 0, 255));
		}
	}
	else {
		_texture = NULL;
	}
}

Sprite& Sprite::operator=(const Sprite& rhs){
	if (_texture)
		al_destroy_bitmap(_texture);

	_id = rhs._id;
	if (rhs._texture != NULL) {
		_texture = al_clone_bitmap(rhs._texture);
		if (_texture == NULL){
			cout << "Warning: sprite " << _id << " is not loaded! (clone error)" << endl;
		}
		else {
			al_convert_mask_to_alpha(_texture, al_map_rgb(255, 0, 255));
		}
	}
	else {
		_texture = NULL;
	}
	return *this;
}

Sprite::~Sprite(){
	if (_texture)
		al_destroy_bitmap(_texture);
}

bool Sprite::checkAlpha(const Point& pos) {
	al_get_pixel(_texture, pos._x, pos._y);
	return true;
}

void Sprite::draw(const Point& pos) {
	if (_texture != NULL){
		al_draw_bitmap(_texture, pos._x, pos._y, 0);
	}
}

void Sprite::drawScaled(const Point& pos, const Point& size) {
	if (_texture != NULL){
		al_draw_scaled_bitmap(_texture, 0, 0, al_get_bitmap_width(_texture), al_get_bitmap_height(_texture),
			pos._x, pos._y, size._x, size._y, 0);
	}
}


// SpriteSheet class
// Default X and Y sprite size = 32
SpriteSheet::SpriteSheet(unsigned id, const char* filename, unsigned last, unsigned rowsize,
	const Point& size) : Sprite(id, filename) {
	_last = last;
	_size = size;
	_rowsize = rowsize;
}

SpriteSheet::SpriteSheet(const SpriteSheet& rhs) : Sprite(rhs) {
	_last = rhs._last;
	_size = rhs._size;
	_rowsize = rhs._rowsize;
}

SpriteSheet& SpriteSheet::operator=(const SpriteSheet& rhs){
	Sprite::operator=(rhs);
	_last = rhs._last;
	_size = rhs._size;
	_rowsize = rhs._rowsize;
	return *this;
}

void SpriteSheet::draw(const Point& pos, unsigned int sprite_id) {
	if (sprite_id > _last){
		cout << "Warning: sprite_id " << sprite_id << " is out of bound! Sprite " << _id << endl;
	}
	if (_texture != NULL){
		Point src(sprite_id % _rowsize, sprite_id / _rowsize);
		src = src * _size;
		al_draw_bitmap_region(_texture, src._x, src._y, _size._x, _size._y, pos._x, pos._y, 0);
	}
}

void SpriteSheet::drawScaled(const Point& pos, unsigned int sprite_id, float zoom) {
	if (sprite_id > _last){
		cout << "Warning: sprite_id " << sprite_id << " is out of bound! Sprite " << _id << endl;
	}
	if (_texture != NULL) {
		Point src(sprite_id % _rowsize, sprite_id / _rowsize);
		src = src * _size;
		if (zoom == 1.0) {
			al_draw_bitmap_region(_texture, src._x, src._y, _size._x, _size._y, pos._x, pos._y, 0);
		} else {
			al_draw_scaled_bitmap(_texture, src._x, src._y, _size._x, _size._y, 
				                  pos._x, pos._y, _size._x*zoom, _size._y*zoom, 0);
		}
	}
}

void SpriteSheet::drawRandom(const Point& pos) {
	unsigned int id = rand() % _last;
	draw(pos, id);
}


// SpriteText class
SpriteText::SpriteText(const char* fontname, unsigned size)
	: _filename(fontname), _fontsize(size) {
	_font = al_load_font(fontname, size, 2);
	if (_font == NULL){
		cout << "Warning: font is not loaded! (" << fontname << ")" << endl;
	}
}

SpriteText::SpriteText(const SpriteText& rhs){
	_fontsize = rhs._fontsize;
	_filename = rhs._filename;
	_font = al_load_font(_filename.c_str(), _fontsize, 2);
	if (_font == NULL){
		cout << "Warning: font is not loaded! (" << _filename << ")" << endl;
	}
}

SpriteText& SpriteText::operator=(const SpriteText& rhs){
	if (_font)
		al_destroy_font(_font);

	_fontsize = rhs._fontsize;
	_filename = rhs._filename;
	_font = al_load_font(_filename.c_str(), _fontsize, 2);
	if (_font == NULL){
		cout << "Warning: font is not loaded! (" << _filename << ")" << endl;
	}
	return *this;
}

SpriteText::~SpriteText(){
	if (_font)
		al_destroy_font(_font);
}

void SpriteText::draw(const char* str, const Point& pos, ALLEGRO_COLOR color) {
	if (_font)
		al_draw_text(_font, color, pos._x, pos._y, 0, str);
}

void SpriteText::draw(string& str, const Point& pos, ALLEGRO_COLOR color) {
	draw(str.c_str(), pos, color);
}


void SpriteText::draw(const Point& text, const Point& pos, ALLEGRO_COLOR color) {
	string objectCoords(to_string(text._x) + ", " + to_string(text._y));
	draw(objectCoords.c_str(), pos, color);
}


// RawSprite class
// Useful for drawing parts of unformatted images
RawSprite::RawSprite(unsigned id, const char* filename, const Point& pos,
	                 const Point& size) : Sprite(id, filename) {
	_pos = pos;
	_size = size;
}

RawSprite::RawSprite(const RawSprite& rhs) : Sprite(rhs) {
	_pos = rhs._pos;
	_size = rhs._size;
}

RawSprite& RawSprite::operator=(const RawSprite& rhs){
	Sprite::operator=(rhs);
	_pos = rhs._pos;
	_size = rhs._size;
	return *this;
}

void RawSprite::draw(const Point& pos) {
	if (_texture != NULL){
		al_draw_bitmap_region(_texture, _pos._x, _pos._y, _size._x, _size._y, pos._x, pos._y, 0);
	}
}

void RawSprite::drawScaled(const Point& pos, float zoom) {
	if (_texture != NULL){
		if (zoom == 1.0) {
			al_draw_bitmap_region(_texture, _pos._x, _pos._y, _size._x, _size._y, pos._x, pos._y, 0);
		} else {
			Point scaled = _size * zoom;
			al_draw_scaled_bitmap(_texture, _pos._x, _pos._y, _size._x, _size._y, 
				                  pos._x, pos._y, scaled._x, scaled._y, 0);
		}
	}
}
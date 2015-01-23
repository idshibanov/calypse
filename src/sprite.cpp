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

void Sprite::draw(int x, int y){
	if (_texture != NULL){
		al_draw_bitmap(_texture, x, y, 0);
	}
}

void Sprite::drawScaled(int x, int y, int width, int height){
	if (_texture != NULL){
		al_draw_scaled_bitmap(_texture, 0, 0, al_get_bitmap_width(_texture), al_get_bitmap_height(_texture),
			x, y, width, height, 0);
	}
}


// SpriteSheet class
// Default X and Y sprite size = 32
SpriteSheet::SpriteSheet(unsigned id, const char* filename, unsigned last, unsigned rowsize,
	unsigned xsize, unsigned ysize) : Sprite(id, filename) {
	_last = last;
	_xsize = xsize;
	_ysize = ysize;
	_rowsize = rowsize;
}

SpriteSheet::SpriteSheet(const SpriteSheet& rhs) : Sprite(rhs) {
	_last = rhs._last;
	_xsize = rhs._xsize;
	_ysize = rhs._ysize;
	_rowsize = rhs._rowsize;
}

SpriteSheet& SpriteSheet::operator=(const SpriteSheet& rhs){
	Sprite::operator=(rhs);
	_last = rhs._last;
	_xsize = rhs._xsize;
	_ysize = rhs._ysize;
	_rowsize = rhs._rowsize;
	return *this;
}

void SpriteSheet::draw(int x, int y, unsigned int sprite_id){
	if (sprite_id > _last){
		cout << "Warning: sprite_id " << sprite_id << " is out of bound! Sprite " << _id << endl;
	}
	if (_texture != NULL){
		int sx = (sprite_id % _rowsize) * _xsize;
		int sy = (sprite_id / _rowsize) * _ysize;
		al_draw_bitmap_region(_texture, sx, sy, _xsize, _ysize, x, y, 0);
	}
}

void SpriteSheet::drawScaled(int x, int y, unsigned int sprite_id, float zoom){
	if (sprite_id > _last){
		cout << "Warning: sprite_id " << sprite_id << " is out of bound! Sprite " << _id << endl;
	}
	if (_texture != NULL){
		int sx = (sprite_id % _rowsize) * _xsize;
		int sy = (sprite_id / _rowsize) * _ysize;
		if (zoom == 1.0) {
			al_draw_bitmap_region(_texture, sx, sy, _xsize, _ysize, x, y, 0);
		}
		else {
			al_draw_scaled_bitmap(_texture, sx, sy, _xsize, _ysize, x, y, _xsize*zoom, _ysize*zoom, 0);
		}
	}
}

void SpriteSheet::drawRandom(int x, int y){
	unsigned int id = rand() % _last;
	draw(x, y, id);
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

void SpriteText::draw(const char* str, int x, int y, ALLEGRO_COLOR color){
	if (_font)
		al_draw_text(_font, color, x, y, 0, str);
}

void SpriteText::draw(string& str, int x, int y, ALLEGRO_COLOR color){
	draw(str.c_str(), x, y, color);
}


// RawSprite class
// Useful for drawing parts of unformatted images
RawSprite::RawSprite(unsigned id, const char* filename, unsigned xpos, unsigned ypos,
	unsigned xsize, unsigned ysize) : Sprite(id, filename) {
	_xsize = xsize;
	_ysize = ysize;
	_xpos = xpos;
	_ypos = ypos;
}

RawSprite::RawSprite(const RawSprite& rhs) : Sprite(rhs) {
	_xsize = rhs._xsize;
	_ysize = rhs._ysize;
	_xpos = rhs._xpos;
	_ypos = rhs._ypos;
}

RawSprite& RawSprite::operator=(const RawSprite& rhs){
	Sprite::operator=(rhs);
	_xsize = rhs._xsize;
	_ysize = rhs._ysize;
	_xpos = rhs._xpos;
	_ypos = rhs._ypos;
	return *this;
}

void RawSprite::draw(int x, int y){
	if (_texture != NULL){
		al_draw_bitmap_region(_texture, _xpos, _ypos, _xsize, _ysize, x, y, 0);
	}
}

void RawSprite::drawScaled(int x, int y, float zoom){
	if (_texture != NULL){
		if (zoom == 1.0) {
			al_draw_bitmap_region(_texture, _xpos, _ypos, _xsize, _ysize, x, y, 0);
		}
		else {
			al_draw_scaled_bitmap(_texture, _xpos, _ypos, _xsize, _ysize, x, y, _xsize*zoom, _ysize*zoom, 0);
		}
	}
}
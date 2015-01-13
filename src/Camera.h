#pragma once
#include "Main.h"

class Camera{
	int _xpos;
	int _ypos;
	int _xmax;
	int _ymax;
public:
	Camera(int xpos, int ypos, unsigned xmax, unsigned ymax) {
		_xpos = xpos;
		_ypos = ypos;
		_xmax = xmax;
		_ymax = ymax;
	}
	~Camera() { }
	int getXPos() { return _xpos; }
	int getYPos() { return _ypos; }
	void move(unsigned dir, unsigned dist = 4) {
		switch (dir){
		case 1:
			_xpos -= dist;
			if (_xpos < 0) _xpos = 0;
			break;
		case 2:
			_xpos += dist;
			if (_xpos > _xmax) _xpos = _xmax;
			break;
		case 3:
			_ypos -= dist;
			if (_ypos < 0) _ypos = 0;
			break;
		case 4:
			_ypos += dist;
			if (_ypos > _ymax) _ypos = _ymax;
			break;

		}
	}
};
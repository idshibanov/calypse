#pragma once
#include "Main.h"

class Camera{
	int _xpos;
	int _ypos;
	int _xmax;
	int _ymax;
	bool _updated;
public:
	Camera(unsigned xmax, unsigned ymax) {
		_xpos = 0;
		_ypos = 0;
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
			_updated = true;
			break;
		case 2:
			_xpos += dist;
			if (_xpos > _xmax) _xpos = _xmax;
			_updated = true;
			break;
		case 3:
			_ypos -= dist;
			if (_ypos < 0) _ypos = 0;
			_updated = true;
			break;
		case 4:
			_ypos += dist;
			if (_ypos > _ymax) _ypos = _ymax;
			_updated = true;
			break;

		}
	}
	bool isUpdated() {
		bool retval = _updated;

		_updated = false;
		return retval;
	}
};
#include "Camera.h"

Camera::Camera(unsigned xmax, unsigned ymax) {
	_xpos = 0;
	_ypos = 0;
	_xmax = xmax;
	_ymax = ymax;
}

Camera::~Camera() {

}

int Camera::getXPos() {
	return _xpos;
}

int Camera::getYPos() { 
	return _ypos; 
}

Point Camera::getPos() { 
	return Point(_xpos, _ypos);
}

void Camera::move(unsigned dir, unsigned dist) {
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

bool Camera::isUpdated() {
	bool retval = _updated;

	_updated = false;
	return retval;
}
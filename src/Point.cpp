#include <algorithm>
#include "Point.h"
#include "Utils.h"


Point::Point() {
	_x = 0;
	_y = 0;
}

Point::Point(int x, int y) {
	_x = x;
	_y = y;
}

Point::Point(const Point& rhs) {
	_x = rhs._x;
	_y = rhs._y;
}

Point::~Point() {
}

Point& Point::set(int x, int y) {
	_x = x;
	_y = y;
	return *this;
}

Point& Point::add(int x, int y) {
	_x += x;
	_y += y;
	return *this;
}

Point& Point::sub(int x, int y) {
	_x -= x;
	_y -= y;
	return *this;
}

Point& Point::mul(int mod) {
	_x *= mod;
	_y *= mod;
	return *this;
}

Point& Point::div(int mod) {
	_x /= mod;
	_y /= mod;
	return *this;
}

Point& Point::inv() {
	_x = -_x;
	_y = -_y;
	return *this;
}

Point& Point::operator= (const Point& rhs) {
	_x = rhs._x;
	_y = rhs._y;
	return *this;
}

Point& Point::operator+= (const Point& rhs) {
	_x += rhs._x;
	_y += rhs._y;
	return *this;
}

Point& Point::operator-= (const Point& rhs) {
	_x -= rhs._x;
	_y -= rhs._y;
	return *this;
}

Point Point::operator+ (const Point& rhs) const {
	return Point(_x + rhs._x, _y + rhs._y);
}

Point Point::operator- (const Point& rhs) const {
	return Point(_x - rhs._x, _y - rhs._y);
}

Point Point::operator* (const Point& rhs) const {
	return Point(_x * rhs._x, _y * rhs._y);
}

Point Point::operator* (int mod) const {
	return Point(_x * mod, _y * mod);
}

Point Point::operator* (double mod) const {
	return Point(_x * mod, _y * mod);
}

Point Point::operator/ (const Point& rhs) const {
	return Point(_x / rhs._x, _y / rhs._y);
}

Point Point::operator/ (int mod) const {
	return Point(_x / mod, _y / mod);
}

Point Point::operator/ (double mod) const {
	return Point(_x / mod, _y / mod);
}

Point Point::operator% (int limit) const {
	return Point(_x % limit, _y % limit);
}

bool Point::operator== (const Point& rhs) const {
	return _x == rhs._x && _y == rhs._y;
}

bool Point::operator!= (const Point& rhs) const {
	return _x != rhs._x || _y != rhs._y;
}

bool Point::operator< (const Point& rhs) const {
	if (_x <= rhs._x && _y < rhs._y) {
		return true;
	} else if (_x < rhs._x && _y <= rhs._y) {
		return true;
	}
	return false;
	//return toRenderPriority() < rhs.toRenderPriority();
}

bool Point::operator> (const Point& rhs) const {
	if (_x >= rhs._x && _y > rhs._y) {
		return true;
	} else if (_x > rhs._x && _y >= rhs._y) {
		return true;
	}
	return false;
	//return toRenderPriority() > rhs.toRenderPriority();
}

size_t Point::operator() (const Point& k) const {
	return 0;
}

int Point::toRenderPriority() const {
	return summation(_x + _y) + _y;
}

int Point::toRenderPriority(const Point& size) const {
	double x = _x + (size._x - 1) / 2.0;
	double y = _y + (size._y - 1) / 2.0;
	return summation(x + y) + y;
}

int Point::toID(int xMax) const {
	return _y * xMax + _x;
}

Point Point::toIso() const {
	return Point(_x - _y, (_x + _y) / 2);
}

Point Point::toMap() const {
	return Point((2 * _y + _x) / 2, (2 * _y - _x) / 2);
}

Point Point::limit(int limit) const {
	int x, y;

	if (_x < 0) {
		x = std::max(_x, -limit);
	} else {
		x = std::min(_x, limit);
	}

	if (_y < 0) {
		y = std::max(_y, -limit);
	} else {
		y = std::min(_y, limit);
	}

	return Point(x, y);
}

void Point::iterate(int max, int min) {
	_x += SUBTILE_MASK;
	if (_x >= max) {
		_y += SUBTILE_MASK;
		_x = min;
	}
}

void Point::convertToIso() {
	_x -= _y;
	_y = (_x + _y) / 2;
}

void Point::convertToMap() {
	_x = (2 * _y + _x) / 2;
	_y = (2 * _y - _x) / 2;
}



Rect::Rect(const Point& pos, const Point& size) {
	_pos = pos;
	_size = size;
}

Rect::~Rect() {
}

void Rect::iterate(std::function<void(const Point&)>& action) {
	Point last = _pos + _size, current;

	for (current = _pos; current < last; current.iterate(last._x)) {
		action(current);
	}
}

bool Rect::iterate(std::function<bool(const Point&)>& action, bool defaultVal) {
	bool retval = defaultVal;
	Point last = _pos + _size, current;

	for (current = _pos; current < last; current.iterate(last._x)) {
		if (action(current) != defaultVal) {
			retval = !retval;
			break;
		}
	}
	return retval;
}
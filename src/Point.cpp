#include <algorithm>
#include "Point.h"
#include "Utils.h"


Point::Point() {
	_x = 0;
	_y = 0;
}

Point::Point(int priority) {
	int rev = reverseSummation(priority);
	int sum = summation(rev);
	_y = priority - sum;
	_x = rev - _y;
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

Point& Point::modAdd(int x, int y) {
	_x += x;
	_y += y;
	return *this;
}

Point& Point::modSub(int x, int y) {
	_x -= x;
	_y -= y;
	return *this;
}

Point& Point::modMul(int mod) {
	_x *= mod;
	_y *= mod;
	return *this;
}

Point& Point::modDiv(int mod) {
	_x /= mod;
	_y /= mod;
	return *this;
}

Point& Point::modInv() {
	_x = -_x;
	_y = -_y;
	return *this;
}

Point Point::add(int x, int y) const {
	return Point(_x + x, _y + y);
}

Point Point::sub(int x, int y) const {
	return Point(_x - x, _y - y);
}

Point Point::mul(int mod) const {
	return Point(_x * mod, _y * mod);
}

Point Point::div(int mod) const {
	return Point(_x / mod, _y / mod);
}

Point Point::inv() const {
	return Point(-_x, -_y);
}

Point Point::absDiff(const Point& pos) const {
	return Point(abs(_x-pos._x), abs(_y-pos._y));
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
	return summation(_x + _y) + _y;
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

void Point::iterate(int max, int min, int step) {
	_x += step;
	if (_x >= max) {
		_y += step;
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

bool Point::in(const Point& pos, const Point& size) const {
	Point max = pos + size;

	if (_x < pos._x || _y < pos._y ||
		_x > max._x || _y > max._y) {
		return false;
	}
	return true;
}

bool Point::in(const Rect& area) const {
	return in(area._pos, area._size);
}

bool Point::in(const Point& center, double radius) const {
	Point diff = absDiff(center);
	diff = diff*diff;
	return sqrt(diff._x + diff._y) <= radius;
}



Rect::Rect() {

}

Rect::Rect(const Point& pos, const Point& size) {
	_pos = pos;
	_size = size;
}

Rect::~Rect() {
}

Rect::Rect(const Rect& rhs) {
	_pos = rhs._pos;
	_size = rhs._size;
}

Rect& Rect::operator=(const Rect& rhs) {
	_pos = rhs._pos;
	_size = rhs._size;
	return *this;
}

bool Rect::contain(const Point& pos) const {
	return pos.in(_pos, _size);
}

void Rect::iterate(std::function<void(const Point&)>& action) {
	Point last = _pos + _size, current;

	for (current = _pos; current < last; current.iterate(last._x, _pos._x)) {
		action(current);
	}
}

bool Rect::iterate(std::function<bool(const Point&)>& action, bool defaultVal) {
	bool retval = defaultVal;
	Point last = _pos + _size, current;

	for (current = _pos; current < last; current.iterate(last._x, _pos._x)) {
		if (action(current) != defaultVal) {
			retval = !retval;
			break;
		}
	}
	return retval;
}
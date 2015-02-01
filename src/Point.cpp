#include "Point.h"


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

bool Point::operator== (const Point& rhs) const {
	return _x == rhs._x && _y == rhs._y;
}

Point Point::toIso() const {
	return Point(_x - _y, (_x + _y) / 2);
}

Point Point::toMap() const {
	return Point((2 * _y + _x) / 2, (2 * _y - _x) / 2);
}

void Point::convertToIso() {
	_x -= _y;
	_y = (_x + _y) / 2;
}

void Point::convertToMap() {
	_x = (2 * _y + _x) / 2;
	_y = (2 * _y - _x) / 2;
}
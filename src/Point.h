#pragma once
#include <functional>
#include "Main.h"

#define TILE_MASK 10
#define SUBTILE_STEPS 5
#define SUBTILE_MASK 2 // 10/5 (integer value, MASK / STEPS)

class Rect;

class Point {
public:
	int _x;
	int _y;
	Point();
	Point(int);
	Point(int, int);
	Point(const Point&);
	~Point();
	Point& set(int, int);
	Point& modAdd(int, int);
	Point& modSub(int, int);
	Point& modMul(int);
	Point& modDiv(int);
	Point& modInv();
	Point add(int, int) const;
	Point sub(int, int) const;
	Point mul(int) const;
	Point div(int) const;
	Point inv() const;
	Point absDiff(const Point&) const;
	Point& operator= (const Point&);
	Point& operator+= (const Point&);
	Point& operator-= (const Point&);
	Point operator+ (const Point&) const;
	Point operator- (const Point&) const;
	Point operator* (const Point&) const;
	Point operator* (int mod) const;
	Point operator* (double mod) const;
	Point operator/ (const Point&) const;
	Point operator/ (int mod) const;
	Point operator/ (double mod) const;
	Point operator% (int limit) const;
	bool operator== (const Point&) const;
	bool operator!= (const Point&) const;
	bool rectLessThan(const Point&) const; // it's not a strict weak ordering; handle with care
	bool rectMoreThan(const Point&) const;
	bool operator> (const Point&) const; // default lexicographical ordering; suitable for std containers
	bool operator< (const Point&) const;
	bool operator> (int) const;
	bool operator< (int) const;
	size_t operator() (const Point& k) const;
	int toRenderPriority() const;
	int toRenderPriority(const Point&) const;
	int toID(int) const;
	Point toIso() const;
	Point toMap() const;
	Point limit(int) const;
	void iterate(int max, int min = 0, int step = SUBTILE_MASK);
	void convertToIso();
	void convertToMap();
	bool in(const Point& pos, const Point& size) const; // rectangle
	bool in(const Rect& area) const; // rectangle
	bool in(const Point& center, double radius) const; // circle
};

class Rect {
public:
	Rect();
	Rect(const Point&, const Point&);
	~Rect();
	Rect(const Rect&);
	Rect& operator= (const Rect&);
	Point _pos;
	Point _size;
	bool contain(const Point& pos) const;
	void iterate(std::function<void(const Point&)>& action, int step = SUBTILE_MASK);
	bool iterate(std::function<bool(const Point&)>& action, bool defaultVal, int step = SUBTILE_MASK);
};


struct cmpPointsStrict {
	bool operator()(const Point& a, const Point& b) const {
		if (a._x != b._x) return a._x < b._x;
		return a._y < b._y;
	}
};
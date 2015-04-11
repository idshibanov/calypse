#pragma once
#include <functional>

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
	bool operator> (const Point&) const;
	bool operator< (const Point&) const;
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
	Rect(const Point&, const Point&);
	~Rect();
	Point _pos;
	Point _size;
	bool contain(const Point& pos) const;
	void iterate(std::function<void(const Point&)>& action);
	bool iterate(std::function<bool(const Point&)>& action, bool defaultVal);
};

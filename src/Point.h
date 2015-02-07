#pragma once
#include <functional>

class Point {
public:
	int _x;
	int _y;
	Point();
	Point(int, int);
	Point(const Point&);
	~Point();
	Point& set(int, int);
	Point& add(int, int);
	Point& sub(int, int);
	Point& mul(int);
	Point& div(int);
	Point& inv();
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
	int toID(int) const;
	Point toIso() const;
	Point toMap() const;
	Point limit(int) const;
	void convertToIso();
	void convertToMap();
};

class Rect {
public:
	Point _pos;
	Point _size;
	void iterate(std::function<void(const Point&)>& action);
};
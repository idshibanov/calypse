#pragma once

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
	bool operator== (const Point&) const;
	int toID(int) const;
	Point toIso() const;
	Point toMap() const;
	void convertToIso();
	void convertToMap();
};
#pragma once
#include "Main.h"
#include "Point.h"

class Camera{
	int _xpos;
	int _ypos;
	int _xmax;
	int _ymax;
	bool _updated;
public:
	Camera(unsigned xmax, unsigned ymax);
	~Camera();
	int getXPos();
	int getYPos();
	Point getPos();
	void move(unsigned dir, unsigned dist = 4);
	bool isUpdated();
};
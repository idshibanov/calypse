#pragma once
#include "ActionType.h"
#include "Point.h"
#include "Main.h"

class UIFrame;
class Item;
class Screen;

struct AppState {
	// statistics
	int _appTime;
	int _CPS;
	int _FPS;

	// settings
	Point _resolution;

	bool _drawStats;
	bool _drawGrid;
	bool _drawCoords;
	bool _drawMasks;
	bool _drawUIAreas;
	int _appSpeed;

	// state
	shared_ptr<Screen> _curScreen;
	ActionType _selectedAction;
	int _selectedObject;
	UIFrame* _selectedFrame;
	shared_ptr<Item> _selectedItem;
};
#pragma once
#include "ActionType.h"
#include "Main.h"

class UIFrame;
class Item;

struct AppState {
	// statistics
	int _appTime;
	int _CPS;
	int _FPS;

	// settings
	bool _drawStats;
	bool _drawGrid;
	bool _drawCoords;
	bool _drawMasks;
	bool _drawUIAreas;
	int _appSpeed;

	// state
	ActionType _selectedAction;
	int _selectedObject;
	UIFrame* _selectedFrame;
	shared_ptr<Item> _selectedItem;
};
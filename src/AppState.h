#pragma once
class UIFrame;

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
	int _selectedAction;
	int _selectedObject;
	UIFrame* _selectedFrame;
};
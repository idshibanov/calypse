#pragma once

struct AppState {
	// statistics
	int _appTime;
	int _CPS;
	int _FPS;

	// settings
	bool _drawGrid;
	bool _drawCoords;
	bool _drawMasks;
	int _appSpeed;

	// state
	int _selectedObject;
};
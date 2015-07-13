#pragma once

/*
MoveAction - MOVE
ObjectAction - CUT, DRAG, PICK_GATHER WOOD/RB/BB
PointAction - DROP, PICK_ITEM, CRAFT TREE/FIRE
ItemAction - ACTION_PICK_ITEM

type, actor, timers, _res

ACTION_ABS_SLEEP,	(Plain)
ACTION_ABS_DROP,	targetPos - (PointAction)
ACTION_ABS_MOVE,	targetPos, _pathfinder - (MoveAction)
ACTION_ABS_GATHER,	targetObj, _map, itemParam - (ObjectAction)
ACTION_ABS_PICK,	item, _map - (ItemAction)
ACTION_ABS_CARRY,	targetObj - (ObjectAction)
ACTION_ABS_CUT,		targetObj, _map - (ObjectAction)
ACTION_ABS_CRAFT_OBJ,	targetPos, objParam, _map - (PointAction)

Action - type, actor, timers, _res, _map
MoveAction - targetPos, pathfinder
PlainAction - JsonValue array
Point
ptr<Item>
ptr<Object>
JsonValue
*/

enum ActionAbstractType {
	ACTION_ABS_MOVE,
	ACTION_ABS_SLEEP,
	ACTION_ABS_GATHER,  // earn
	ACTION_ABS_PICK,    // from ground
	ACTION_ABS_CARRY,   // pick up object
	ACTION_ABS_DROP,    // place objects
	ACTION_ABS_CUT,
	ACTION_ABS_CRAFT,
	ACTION_ABSTRACT_LAST,
};

enum ActionType {

};

/*
enum ActionType {
	ACTION_NONE,
	ACTION_CLOSE_PARENT,
	ACTION_MOVE,
	ACTION_DRAG,
	ACTION_DROP,
	ACTION_PICK_ITEM,
	ACTION_CRAFT_CAMPFIRE,
	ACTION_CRAFT_TREE,
	ACTION_CUT,
	ACTION_PICK_BRANCH,
	ACTION_PICK_RED_BERRY,
	ACTION_PICK_BLUE_BERRY,
	ACTION_LAST
};
*/
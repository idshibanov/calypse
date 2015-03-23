#pragma once
#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

#define TD_TILESIZE_X 32
#define TD_TILESIZE_Y 32

// TODO: task / UI / general animation (clock) / idle animation / object stats
// Nice to have: better PF, normalized speed, smooth animation, drag&drop, active areas, AI
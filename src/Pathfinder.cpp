#include "Map.h"
#include "Pathfinder.h"

AStarNode::AStarNode(const Point& p) : _pos(p) {
	_parent = nullptr;
	_closed = false;
	_f = 0;
	_g = 0;
	_h = 0;
}

bool AStarNode::operator< (const AStarNode& rhs) const {
	return rhs._f < this->_f;
}

// bool AStarNode::operator() (const AStarNode& lhs, const AStarNode& rhs) const {
//     return lhs._f < rhs._f;
// }

bool AStarNode::operator== (const AStarNode& rhs) const {
	return _pos == rhs._pos;
}

std::ostream& operator<<(std::ostream& out, const AStarNode& rhs) {
	out << "Node: " << rhs._pos._x << "," << rhs._pos._y;
	out << "  g: " << rhs._g << " h: " << rhs._h;
	return out;
}


// STL priority_queue hack - searching through the open set
// function is const, so could be considered safe

template<typename T>
bool AStarHeap<T>::contains(AStarNode& node) const {
	bool found = false;
	for (auto it = c.begin(); !found && it != c.end(); it++) {
		if (node == (*it))
			found = true;
	}
	return found;
}


AStarSearch::AStarSearch(shared_ptr<LocalMap> map) {
	_map = map;
}

AStarSearch::~AStarSearch() {

}

std::vector<shared_ptr<AStarNode>> AStarSearch::getNeighbors(const AStarNode& node) {
	std::vector<shared_ptr<AStarNode>> nodesFound;

	std::function<void(const Point&)> nSearch = [this, &nodesFound, &node](const Point& neighbor) {
		if (neighbor != node._pos && _map->tileIsFree(neighbor)) {
			nodesFound.push_back(make_shared<AStarNode>(neighbor));
		}
	};

	Rect area(node._pos.sub(1, 1), Point(3, 3));
	area.iterate(nSearch, 1);

	return nodesFound;
}

size_t AStarSearch::heuristicCost(const AStarNode& start, const AStarNode& goal) const {
	size_t diffX = abs((long)(start._pos._x - goal._pos._x));
	size_t diffY = abs((long)(start._pos._y - goal._pos._y));
	//return max(diffX, diffY) * PATHFINDING_MOVE_COST;
	return PATHFINDING_MOVE_COST * (diffX + diffY) + min(diffX, diffY) * (PATHFINDING_DIAGONAL_COST - 2 * PATHFINDING_MOVE_COST);
}

void AStarSearch::recursePath(shared_ptr<AStarNode>& node) {
	if (node->_parent) {
		recursePath(node->_parent);
	}
	_pathFound.push_back(*node);
}

Point AStarSearch::findAdjacent(const Point& start, const Rect& target) const {
	Point retval(-1, -1);
	AStarHeap<AStarNode> adjSet;
	AStarNode startNode(start);

	Rect area(target._pos.sub(SUBTILE_MASK, SUBTILE_MASK), target._size.add(SUBTILE_MASK, SUBTILE_MASK));

	std::function<void(const Point&)> setter = [&, this](const Point& pos) {
		if (!target.contain(pos)) {
			if (_map->isFree(pos)) {
				AStarNode current(pos);
				current._f = heuristicCost(startNode, current);
				adjSet.push(current);
			}
		}
	};
	area.iterate(setter);

	if (!adjSet.empty()) {
		retval = adjSet.top()._pos;
	}
	return retval;
}

void AStarSearch::clearData() {
	while (!_openSet.empty())
		_openSet.pop();
	_visitedSet.clear();
	_pathFound.clear();
}

std::vector<AStarNode> AStarSearch::searchPath(const Point& start, const Point& goal) {
	// ensure we're not messing with previous data
	clearData();

	//if (_map->tileIsFree(goal._x / TILE_MASK, goal._y / TILE_MASK)) {
		AStarNode startNode(start / TILE_MASK);
		AStarNode goalNode(goal / TILE_MASK);
		_openSet.push(startNode);
		shared_ptr<AStarNode> closest = nullptr;
		size_t max_h = heuristicCost(startNode, goalNode);

		while (!_openSet.empty()) {
			shared_ptr<AStarNode> current = make_shared<AStarNode>(_openSet.top());

			if (*current == goalNode) {
				recursePath(current);
				return _pathFound;
			}

			current->_closed = true;
			_visitedSet.push_back(current);
			_openSet.pop();
			std::vector<shared_ptr<AStarNode>>& neighbors = getNeighbors(*current);

			for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
				shared_ptr<AStarNode> n = (*it);
				unsigned g = current->_g + PATHFINDING_MOVE_COST;
				bool beenVisited = false;

				for (auto itVisit = _visitedSet.begin(); !beenVisited && itVisit != _visitedSet.end(); ++itVisit) {
					if (*n == *(*itVisit)) {
						beenVisited = true;
						if ((*itVisit)->_closed) {
							n->_closed = true;
						}
					}
				}

				if (!n->_closed && (!beenVisited || g < n->_g)) {
					n->_parent = current;
					n->_g = g;
					if (n->_h == 0)
						n->_h = heuristicCost(*n, goalNode);
					n->_f = n->_g + n->_h;

					_visitedSet.push_back(n);
					bool isOpen = _openSet.contains(*n);
					if (!isOpen && n->_h < max_h) {
						_openSet.push(*n);
						if (closest == nullptr || closest->_h > n->_h) {
							closest = n;
						}
					}
				}
			}
		}
	//}
	if (closest) {
		recursePath(closest);
	}
	return _pathFound;
}
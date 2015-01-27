#include "Pathfinder.h"

AStarNode::AStarNode(unsigned x, unsigned y) {
	_parent = nullptr;
	_closed = false;
	_mapX = x;
	_mapY = y;
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
	return this->_mapX == rhs._mapX && this->_mapY == rhs._mapY;
}

ostream& operator<<(ostream& out, const AStarNode& rhs) {
	out << "Node: " << rhs._mapX << "," << rhs._mapY;
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

std::vector<shared_ptr<AStarNode>> AStarSearch::getNeighbors(AStarNode& node, AStarNode& goal) {
	std::vector<shared_ptr<AStarNode>> nodesFound;


	// WEST NODE (4)
	if (_map->tileIsFree(node._mapX - 1, node._mapY)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX - 1, node._mapY));
	}

	// EAST NODE (6)
	if (_map->tileIsFree(node._mapX + 1, node._mapY)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX + 1, node._mapY));
	}

	// NORTH-WEST NODE (7)
	if (_map->tileIsFree(node._mapX - 1, node._mapY - 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX - 1, node._mapY - 1));
	}

	// NORTH NODE (8)
	if (_map->tileIsFree(node._mapX, node._mapY - 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX, node._mapY - 1));
	}

	// NORTH-EAST NODE (9)
	if (_map->tileIsFree(node._mapX + 1, node._mapY - 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX + 1, node._mapY - 1));
	}

	// SOUTH-WEST NODE (1)
	if (_map->tileIsFree(node._mapX - 1, node._mapY + 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX - 1, node._mapY + 1));
	}

	// SOUTH NODE (2)
	if (_map->tileIsFree(node._mapX, node._mapY + 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX, node._mapY + 1));
	}

	// SOUTH-EAST NODE (3)
	if (_map->tileIsFree(node._mapX + 1, node._mapY + 1)) {
		nodesFound.push_back(make_shared<AStarNode>(node._mapX + 1, node._mapY + 1));
	}

	return nodesFound;
}

size_t AStarSearch::heuristicCost(AStarNode& start, AStarNode& goal) {
	size_t diffX = abs((long)(start._mapX - goal._mapX));
	size_t diffY = abs((long)(start._mapY - goal._mapY));
	//return max(diffX, diffY) * PATHFINDING_MOVE_COST;
	return PATHFINDING_MOVE_COST * (diffX + diffY) + min(diffX, diffY) * (PATHFINDING_DIAGONAL_COST - 2 * PATHFINDING_MOVE_COST);
}

size_t AStarSearch::heuristicCost(size_t startID, size_t goalID) {
	size_t diffX = abs((long)(_map->convertIDToX(startID) - _map->convertIDToX(goalID)));
	size_t diffY = abs((long)(_map->convertIDToY(startID) - _map->convertIDToY(goalID)));
	//return max(diffX, diffY) * PATHFINDING_MOVE_COST;
	return PATHFINDING_MOVE_COST * (diffX + diffY) + min(diffX, diffY) * (PATHFINDING_DIAGONAL_COST - 2 * PATHFINDING_MOVE_COST);
}

void AStarSearch::recursePath(shared_ptr<AStarNode>& node) {
	if (node->_parent) {
		recursePath(node->_parent);
	}
	_pathFound.push_back(*node);
}

void AStarSearch::clearData() {
	while (!_openSet.empty())
		_openSet.pop();
	_visitedSet.clear();
	_pathFound.clear();
}

std::vector<AStarNode> AStarSearch::searchPath(size_t startID, size_t goalID) {
	return searchPath(_map->convertIDToX(startID), _map->convertIDToY(startID), _map->convertIDToX(goalID), _map->convertIDToY(goalID));
}

std::vector<AStarNode> AStarSearch::searchPath(size_t startX, size_t startY, size_t goalX, size_t goalY) {
	// ensure we're not messing with previous data
	clearData();

	AStarNode startNode(startX / TILE_MASK, startY / TILE_MASK);
	AStarNode goalNode(goalX / TILE_MASK, goalY / TILE_MASK);
	_openSet.push(startNode);

	while (!_openSet.empty()) {
		shared_ptr<AStarNode> current = make_shared<AStarNode>(_openSet.top());

		if (*current == goalNode) {
			recursePath(current);
			return _pathFound;
		}

		current->_closed = true;
		_visitedSet.push_back(current);
		_openSet.pop();
		std::vector<shared_ptr<AStarNode>>& neighbors = getNeighbors(*current, goalNode);

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
				if (!isOpen)
					_openSet.push(*n);
			}
		}
	}

	return _pathFound;
}
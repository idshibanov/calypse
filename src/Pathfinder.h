#pragma once
#include <queue>

#define PATHFINDING_MOVE_COST 10
#define PATHFINDING_DIAGONAL_COST 14

// Pathfinder is not thread safe! 
// Implement mutex if will go for multithreading and definetely server-side.

class LocalMap;

class AStarNode {
public:
	std::shared_ptr<AStarNode> _parent;
	Point _pos;
	size_t _f; // total cost
	size_t _g; // current cost
	size_t _h; // heuristic_cost
	bool _closed;
	AStarNode(const Point&);
	bool operator< (const AStarNode& rhs) const;
	//    bool operator() (const AStarNode& lhs, const AStarNode& rhs) const;
	bool operator== (const AStarNode& rhs) const;
	friend std::ostream& operator<<(std::ostream& out, const AStarNode& rhs);
};

template<typename T>
class AStarHeap : public std::priority_queue < T > {
public:
	bool contains(AStarNode&) const;
};

class AStarSearch {
	std::shared_ptr<LocalMap> _map;
	AStarHeap<AStarNode> _openSet;
	std::vector<std::shared_ptr<AStarNode>> _visitedSet;
	std::vector<AStarNode> _pathFound;
public:
	AStarSearch(std::shared_ptr<LocalMap> map);
	~AStarSearch();
	std::vector<std::shared_ptr<AStarNode>> getNeighbors(AStarNode& node, AStarNode& goal);
	size_t heuristicCost(const AStarNode& start, const AStarNode& goal) const;
	void recursePath(std::shared_ptr<AStarNode>& node);
	Point findAdjacent(const Point& start, const Rect& target) const;
	void clearData();
	std::vector<AStarNode> searchPath(const Point& start, const Point& goal);
};
#ifndef WORLD_GRAPH_H
#define WORLD_GRAPH_H

#include "Location.h"
#include "Orientation.h"
#include <vector>
//Board squares handle state information that is not directly related to
//the agent.

struct BoardSquare{

	BoardSquare * above;
	BoardSquare * below;
	BoardSquare * left;
	BoardSquare * right;

	bool stench;
	bool breeze;
	bool pit;
	bool gold;
	bool wumpus;

	Orientation wall;//Gotta make sure this doesnt initialize to bad
	Location location;

};

bool operator==(const BoardSquare *lhs, const BoardSquare *rhs) {
	return lhs->location == rhs->location;
}

class WorldGraph {
public:

	WorldGraph();
	~WorldGraph();//Gonna need some good cleanup to avoid memory leaks

	BoardSquare * initialSquare;
	BoardSquare * currentLocation;
	BoardSquare * previousLocation;

	void newBoardSquare(Orientation agentOrientation);

	void deleteBoardSquares(BoardSquare * initialSquare);

	std::vector<*BoardSquare> exploredSet;

	std::vector<*BoardSquare>::iterator inExploredSet(BoardSquare *test);
};




#endif

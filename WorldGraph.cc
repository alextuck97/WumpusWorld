#include "WorldGraph.h"

WorldGraph::WorldGraph()
{
	initialSquare->location = Location(1, 1);
}

WorldGraph::~WorldGraph()
{
	deleteBoardSquares(initialSquare);
}

//Recursively delete the entire graph
void WorldGraph::deleteBoardSquares(BoardSquare * initialSquare)
{
	if (initialSquare == nullptr)
		return;

	BoardSquare * left = initialSquare->left;
	BoardSquare * right = initialSquare->right;
	BoardSquare * above = initialSquare->above;
	BoardSquare * below = initialSquare->below;

	delete initialSquare;
	deleteBoardSquares(left);
	deleteBoardSquares(right);
	deleteBoardSquares(above);
	deleteBoardSquares(below);
}

//Creates a new board square for the new current location of the agent. Updates
//the previous location to have a pointer in the direction of new board space.
//Ideally, should check for existence of board piece.
void WorldGraph::newBoardSquare(Orientation agentOrientation) {
	
	this->previousLocation = this->currentLocation;
	this->currentLocation = new BoardSquare();
	
	if (agentOrientation == LEFT) {
		this->currentLocation = Location(previousLocation->location.X - 1, previousLocation->location.Y);
	}
	else if (agentOrientation == RIGHT) {
		this->currentLocation = Location(previousLocation->location.X + 1, previousLocation->location.Y);
	}
	else if (agentOrientation == UP) {
		this->currentLocation = Location(previousLocation->location.X, previousLocation->location.Y + 1);
	}
	else if (agentOrientation == DOWN) {
		this->currentLocation = Location(previousLocation->location.X, previousLocation->location.Y - 1);
	}

	if (exploredSet.end() == inExploredSet(currentLocation)) {

	}
	else {
		previousLocation->left = currentLocation;
		currentLocation->right = previousLocation;
	}
}

std::vector<*BoardSquare>::iterator WorldGraph::inExploredSet(BoardSquare * test) {
	
	std::vector<*BoardSquare>::iterator it;
	it = std::find(exploredSet.begin(), exploredSet.end(), test);
	return it;
}

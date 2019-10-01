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
void WorldGraph::deleteBoardSquares(BoardSquare *initialSquare)
{
	if (initialSquare == nullptr)
		return;

	BoardSquare *left = initialSquare->left;
	BoardSquare *right = initialSquare->right;
	BoardSquare *above = initialSquare->above;
	BoardSquare *below = initialSquare->below;

	delete initialSquare;
	deleteBoardSquares(left);
	deleteBoardSquares(right);
	deleteBoardSquares(above);
	deleteBoardSquares(below);
}

//Creates a new board square for the new current location of the agent. Updates
//the previous location to have a pointer in the direction of new board space.
//Connects the new piece to the previous piece
//Ideally, should check for existence of board piece.
void WorldGraph::newBoardSquare(Orientation agentOrientation)
{

	this->previousLocation = this->currentLocation;
	this->currentLocation = new BoardSquare();
	//create a new location for the newly arrived at space
	if (agentOrientation == LEFT)
	{
		this->currentLocation->location = Location(previousLocation->location.X - 1, previousLocation->location.Y);
	}
	else if (agentOrientation == RIGHT)
	{
		this->currentLocation->location = Location(previousLocation->location.X + 1, previousLocation->location.Y);
	}
	else if (agentOrientation == UP)
	{
		this->currentLocation->location = Location(previousLocation->location.X, previousLocation->location.Y + 1);
	}
	else if (agentOrientation == DOWN)
	{
		this->currentLocation->location = Location(previousLocation->location.X, previousLocation->location.Y - 1);
	}
	
	//search for the current location in the explored set
	std::vector<BoardSquare *>::iterator isExplored = inExploredSet(currentLocation);	
	//Not in the explored set
	if (exploredSet.end() == isExplored)
	{
		//Attach the newly made location to previous location
		switch (agentOrientation)
		{
		case LEFT:
			currentLocation->right = previousLocation;
			previousLocation->left = currentLocation;
			break;
		case RIGHT:
			currentLocation->left = previousLocation;
			previousLocation->right = currentLocation;
			break;
		case UP:
			currentLocation->below = previousLocation;
			previousLocation->above = currentLocation;
			break;
		case DOWN:
			currentLocation->above = previousLocation;
			previousLocation->below = currentLocation;
			break;
		}
	}
	//In the explored set
	else
	{
		//currentLocation already exists in exploredSet, attach the board square
		//contained in isExplored to previousLocation; 
		delete currentLocation;
		currentLocation = (*isExplored);
		switch (agentOrientation)
		{
		case LEFT:
			(*isExplored)->right = previousLocation;
			previousLocation->left = (*isExplored);
			break;
		case RIGHT:
			(*isExplored)->left = previousLocation;
			previousLocation->right = (*isExplored);
			break;
		case UP:
			(*isExplored)->below = previousLocation;
			previousLocation->above = (*isExplored);
			break;
		case DOWN:
			(*isExplored)->above = previousLocation;
			previousLocation->below = (*isExplored);
			break;
		}
	}
}

//Need to test, not sure how == will work with this one
std::vector<BoardSquare *>::iterator WorldGraph::inExploredSet(BoardSquare *test)
{

	std::vector<BoardSquare *>::iterator it;
	it = std::find(exploredSet.begin(), exploredSet.end(), test);

	return it;
}

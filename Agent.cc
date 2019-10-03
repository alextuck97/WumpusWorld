// Agent.cc

#include <iostream>
#include "Agent.h"

using namespace std;

Location operator+(Location &lhs, Location &rhs)
{
	int X = lhs.X + rhs.X;
	int Y = lhs.Y + rhs.Y;
	return Location(X, Y);
}

//If a component of lhs - rhs is 0, they have the same x or y coord,
//implying a stench exists between them
Location operator-(Location &lhs, Location &rhs)
{
	int X = lhs.X - rhs.X;
	int Y = lhs.Y - rhs.Y;
	return Location(X, Y);
}

Agent::Agent()
{
}

Agent::~Agent()
{
	SaveWorldInformation(this->state);
}

void Agent::Initialize()
{

	testWumpusLocator(); //Passed the tests
	getchar();
	state.hasGold = false;
	state.wumpusLocation = Location(-1, -1);
	state.goldLocation = Location(-1, -1);
	state.agentLocation = Location(1, 1);
	state.worldSize = -1;
	state.orientation = RIGHT;
	state.previousAction = TURNLEFT;

	ReadWorldInformation(this->state);
}

Action Agent::Process(Percept &percept)
{
	Action action;
	
	if (!actionQueue.empty())
	{
		action = actionQueue.front();
		actionQueue.pop();
	}
	else
	{
		if (state.goldLocation == Location(-1, -1))
		{
			action = searchForGold(percept);
		}
		else
		{
			action = getGoldAndGetOut(percept);
		}
	}

	if(action == GOFORWARD)
	{
		updateAgentLocation(state.orientation);
	}
	return action;
}

void Agent::GameOver(int score)
{
	if (!(state.agentLocation == Location(1, 1)))
	{
		state.wumpusLocation = state.agentLocation;
	}
}

Action Agent::getGoldAndGetOut(Percept &percept)
{
	Action action;
	//Try to locate the wumpus
	if (state.wumpusLocation == Location(-1, -1))
	{
		handleStench(percept.Stench);
		state.wumpusLocation = locateWumpus(state.wumpusLocation);
	}

	//Gold location is known
	//Don't care if wumpus is known or not because if agent dies
	//it will update its knowledge base with the location of the
	//wumpus.
	if(!percept.Glitter)
	{
		Location direction;
		if (state.hasGold)
		{
			direction = Location(1, 1) - state.agentLocation;
		}
		else
		{
			direction = state.goldLocation - state.agentLocation;
		}


		if (direction.X != 0)
		{
			//Move toward exit
			if (direction.X < 0)
			{	
				if(state.agentLocation + Location(-1,0) == state.wumpusLocation)
				{
					orientateDown(state.orientation);
					actionQueue.push(GOFORWARD);
				}
				else
				{
					orientateLeft(state.orientation);
					actionQueue.push(GOFORWARD);
				}
			}
			//Move toward gold
			else if (direction.X > 0)
			{
				if(state.agentLocation + Location(1,0) == state.wumpusLocation)
				{
					orientateUp(state.orientation);
					actionQueue.push(GOFORWARD);
				}
				else
				{
					orientateRight(state.orientation);
					actionQueue.push(GOFORWARD);
				}
			}
		}
		else if (direction.Y != 0)
		{
			// moveToward exit
			if (direction.Y < 0)
			{
				if(state.agentLocation + Location(0,-1) == state.wumpusLocation)
				{
					orientateRight(state.orientation);
					actionQueue.push(GOFORWARD);
					orientateDown(state.orientation);
					actionQueue.push(GOFORWARD);
					actionQueue.push(GOFORWARD);
				}
				else
				{
					orientateDown(state.orientation);
					actionQueue.push(GOFORWARD);
				}
			}
			//Move toward gold
			else if (direction.Y > 0)
			{
				if(state.agentLocation + Location(0,1) == state.wumpusLocation)
				{
					if(state.wumpusLocation.X != 1)
					{
						orientateLeft(state.orientation);
						actionQueue.push(GOFORWARD);
					}
					//Wumpus is on left most edge and need fancy footwork to avoid it
					else
					{
						orientateRight(state.orientation);
						actionQueue.push(GOFORWARD);
						orientateUp(state.orientation);
						actionQueue.push(GOFORWARD);
						actionQueue.push(GOFORWARD);
						orientateLeft(state.orientation);
						actionQueue.push(GOFORWARD);
					}
					orientateUp(state.orientation);
					actionQueue.push(GOFORWARD);
				}
				else
				{
					orientateUp(state.orientation);
					actionQueue.push(GOFORWARD);
				}
			}
		}
		
	}
	else
	{
		action = handleGlitter(percept.Glitter);
	}
	return action;
}

//Push stench back into stench vector if it hasn't been discovered
//Push normal square into nonstench vector if it hasn't been discovered
void Agent::handleStench(bool stench)
{
	if (stench)
	{
		bool found = false;
		for (int i = 0; i < state.stenches.size(); i++)
		{
			if (state.stenches.at(i) == state.agentLocation)
			{
				found = true;
				break;
			}
		}
		if (!found)
			state.stenches.push_back(state.agentLocation);
	}
	else
	{
		bool found = false;
		for (int i = 0; i < state.notStenches.size(); i++)
		{
			if (state.notStenches.at(i) == state.agentLocation)
			{
				found = true;
				break;
			}
		}
		if (!found)
			state.notStenches.push_back(state.agentLocation);
	}
}

Action Agent::handleGlitter(bool glitter)
{
	Action action;
	if (glitter)
	{
		action = GRAB;
		state.hasGold = true;
		state.goldLocation = state.agentLocation;
	}
	return action;
}

Location Agent::locateWumpus(Location wumpusLocation)
{

	if (wumpusLocation == Location(-1, -1))
	{
		if (state.stenches.size() >= 3)
		{
			//Can locate
			Location difference0 = state.stenches.at(0) - state.stenches.at(1);
			Location difference1 = state.stenches.at(0) - state.stenches.at(2);

			Location w;
			//Because the X or Y component is the same, the average will give
			//the coordinate between the location component that was not equal,
			//the wumpus location
			if (difference0.X == 0 || difference0.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(1);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}
			else if (difference1.X == 0 || difference1.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(2);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}

			return w;
		}
		else if (state.stenches.size() >= 2)
		{
			Location difference0 = state.stenches.at(0) - state.stenches.at(1);
			Location w = Location(-1, -1);
			//Wumpus directly between stenches
			if (difference0.X == 0 || difference0.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(1);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}
			//Stenches are diagonal
			else
			{
				Location s0 = state.stenches.at(0);
				Location s1 = state.stenches.at(1);

				for (int i = 0; i < state.notStenches.size(); i++)
				{
					Location notS = state.notStenches.at(i);
					if (Adjacent(s0, notS) && Adjacent(s1, notS))
					{
						Location dir0 = s0 - notS;
						Location dir1 = s1 - notS;
						w = notS + dir0;
						w = w + dir1;
					}
				}
			}
			return w;
		}
		else
		{ //One stench, no chance
			return wumpusLocation;
		}
	}
	//Already know location
	else
	{
		return wumpusLocation;
	}
}

void Agent::testWumpusLocator()
{
	//Two stenches with block in between case
	state.stenches.clear();
	state.stenches.push_back(Location(2, 1));
	state.stenches.push_back(Location(2, 3));
	Location loc = locateWumpus(Location(-1, -1));

	printf("Size: %d S1: %d, %d S2: %d, %d W: %d, %d\n", 2, 2, 1, 2, 3, loc.X, loc.Y);

	//Three stenches case
	state.stenches.clear();
	state.stenches.push_back(Location(2, 1));
	state.stenches.push_back(Location(4, 1));
	state.stenches.push_back(Location(3, 2));
	loc = locateWumpus(Location(-1, -1));
	printf("Size: %d S1: %d, %d S2: %d, %d S3: %d, %d W: %d, %d\n", 3, 2, 1, 4, 1, 3, 2, loc.X, loc.Y);

	//Two stenches with an adjacent nonStench block case
	state.stenches.clear();
	state.stenches.push_back(Location(3, 2));
	state.stenches.push_back(Location(2, 1));
	state.notStenches.push_back(Location(2, 2));
	loc = locateWumpus(Location(-1, -1));
	printf("Size: %d S1: %d, %d S2: %d, %d NS: %d, %d W: %d, %d\n", 2, 3, 2, 2, 1, 2, 2, loc.X, loc.Y);

	//Two stenches with an adjacent nonStench block case
	state.stenches.clear();
	state.stenches.push_back(Location(3, 2));
	state.stenches.push_back(Location(4, 3));
	state.notStenches.push_back(Location(4, 2));
	loc = locateWumpus(Location(-1, -1));
	printf("Size: %d S1: %d, %d S2: %d, %d NS: %d, %d W: %d, %d\n", 2, 3, 2, 4, 3, 4, 2, loc.X, loc.Y);

	//Two stenches with a non-adjacent nonStench block case
	state.stenches.clear();
	state.notStenches.clear();
	state.stenches.push_back(Location(3, 2));
	state.stenches.push_back(Location(2, 1));
	state.notStenches.push_back(Location(2, 4));
	loc = locateWumpus(Location(-1, -1));
	printf("Size: %d S1: %d, %d S2: %d, %d NS: %d, %d W: %d, %d\n", 2, 3, 2, 2, 1, 2, 4, loc.X, loc.Y);

	//One stench
	state.stenches.clear();
	state.stenches.push_back(Location(3, 2));
	loc = locateWumpus(Location(-1, -1));
	printf("Size: %d S1: %d, %d W: %d, %d\n", 1, 3, 2, loc.X, loc.Y);
}

void Agent::ReadWorldInformation(State &theState)
{
	char *s = "WorldData.txt";
	ifstream f;
	f.open(s);
	if (f.is_open())
	{
		std::string what;
		int X, Y;

		while (f >> what >> X >> Y)
		{
			if (what.compare("GOLDLOCATION") == 0)
			{
				theState.goldLocation.X = X;
				state.goldLocation.Y = Y;
			}
			else if (what.compare("WUMPUSLOCATION") == 0)
			{
				theState.wumpusLocation.X = X;
				theState.wumpusLocation.Y = Y;
			}
		}

		f.close();
	}
	else
	{
		printf("Error opening %s\n", s);
	}
}

void Agent::SaveWorldInformation(State theState)
{
	char *s = "WorldData.txt";
	ofstream f(s, std::ios::out | std::ios::trunc);

	if (f.is_open())
	{
		f << "GOLDLOCATION " << theState.goldLocation.X << " " << theState.goldLocation.Y << std::endl;
		f << "WUMPUSLOCATION " << theState.wumpusLocation.X << " " << theState.wumpusLocation.Y << std::endl;
		f.close();
	}
	else
	{
		printf("Error opening %s\n", s);
	}
}


void Agent::orientateLeft(Orientation o)
{
	if(o == LEFT)
		return;
	if(o == RIGHT)
	{
		actionQueue.push(TURNLEFT);
		actionQueue.push(TURNLEFT);
	}
	else if(o == UP)
	{
		actionQueue.push(TURNLEFT);
	}
	else if(o == DOWN)
	{
		actionQueue.push(TURNRIGHT);
	}
	state.orientation = LEFT;
	return;
}

void Agent::orientateRight(Orientation o)
{
	if(o == RIGHT)
		return;
	if(o == LEFT)
	{
		actionQueue.push(TURNLEFT);
		actionQueue.push(TURNLEFT);
	}
	else if(o == UP)
	{
		actionQueue.push(TURNRIGHT);
	}
	else if(o == DOWN)
	{
		actionQueue.push(TURNLEFT);
	}
	state.orientation = RIGHT;
	return;
}

void Agent::orientateUp(Orientation o)
{
	if(o == UP)
		return;
	if(o == DOWN)
	{
		actionQueue.push(TURNLEFT);
		actionQueue.push(TURNLEFT);
	}
	else if(o == LEFT)
	{
		actionQueue.push(TURNRIGHT);
	}
	else if(o == RIGHT)
	{
		actionQueue.push(TURNLEFT);
	}
	state.orientation = UP;
	return;
}

void Agent::orientateDown(Orientation o)
{
	if(o == DOWN)
		return;
	if(o == UP)
	{
		actionQueue.push(TURNLEFT);
		actionQueue.push(TURNLEFT);
	}
	else if(o == LEFT)
	{
		actionQueue.push(TURNLEFT);
	}
	else if(o == RIGHT)
	{
		actionQueue.push(TURNRIGHT);
	}
	state.orientation = DOWN;
	return;
}


void Agent::updateAgentLocation(Orientation o)
{
	if(o == UP)
	{
		state.agentLocation.Y += 1;
	}
	else if(o == DOWN)
	{
		state.agentLocation.Y -=1;
	}
	else if(o == LEFT)
	{
		state.agentLocation.X -= 1;
	}
	else if(o == RIGHT)
	{
		state.agentLocation.X += 1;
	}
}
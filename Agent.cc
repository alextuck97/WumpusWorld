// Agent.cc



#include "Agent.h"

using namespace std;

Location operator+(Location lhs, Location rhs)
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
	
}

void Agent::Initialize()
{
	
	
}

Action Agent::Process(Percept &percept)
{
	
	static Action action;

	if(action == GOFORWARD && !percept.Bump)
	{
		updateAgentLocation(state.orientation);
	}

	if (!actionQueue.empty())
	{
		action = actionQueue.front();
		actionQueue.pop();
	}
	else
	{
		updateState(percept);
		action = selectAction(percept);
	}
	
	return action;
}



void Agent::GameOver(int score)
{

}


Action selectAction(Percept &percept)
{
	
}


void Agent::updateState(Percept &percept)
{
	//Update breeze information
	if(percept.Breeze)
	{
		auto it = std::find(state.breeze.begin(), state.breeze.end(), state.agentLocation);
		if(it == state.breeze.end())
			state.breeze.push_back(state.agentLocation);
	}

	//Update stench information
	if(percept.Stench)
	{
		auto it = std::find(state.stenches.begin(), state.stenches.end(), state.agentLocation);
		if(it == state.stenches.end())
			state.stenches.push_back(state.agentLocation);
	}

	//Update locations that are not pits. We didnt die when we moved so clearly the
	//current location is not a pit.
	auto it = std::find(state.known_not_pits.begin(), state.known_not_pits.end(), state.agentLocation);
	if(it == state.known_not_pits.end())
		state.known_not_pits.push_back(state.agentLocation);

	//Remove current location from frontier if it is in the frontier
	it = std::find(state.frontier.begin(), state.frontier.end(), state.agentLocation);
	if(it != state.frontier.end())
		state.frontier.erase(it);

	//Add locations adjacent to current location to the frontier if they are not known
	//and they are not already in the frontier
	std::vector<Location> adjacents = getAdjacentLocations(state.agentLocation);
	for(it = adjacents.begin(); it != adjacents.end(); it++)
	{
		auto it_frontier = std::find(state.frontier.begin(), state.frontier.end(), *it);
		auto it_known_no_pits = std::find(state.known_not_pits.begin(), state.known_not_pits.end(), *it);
		auto it_known_pits = std::find(state.known_pits.begin(),state.known_pits.end(), *it);

		if((it_frontier == state.frontier.end()) && it_known_no_pits == state.known_not_pits.end() &&
		it_known_pits == state.known_pits.end())
		{
			state.frontier.push_back(*it);
		} 

		
	}
	
}

//Return a vector of valid locations adjacent to l
std::vector<Location> Agent::getAdjacentLocations(Location l)
{
	std::vector<Location> adj;
	Location left = l + Location(-1,0);
	Location up = l + Location(0,1);
	Location right = l + Location(1,0);
	Location down = l + Location(0,-1);

	for(auto it = adj.begin(); it != adj.end(); it++)
	{
		if(it->X >= 0 && it->Y >= 0 && it->X < WORLD_SIZE && it->Y < WORLD_SIZE)//Add something about world size
			adj.push_back(*it);
	}

	return adj;
}



void Agent::goLeft(Orientation o)
{
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
	actionQueue.push(GOFORWARD);
	return;
}

void Agent::goRight(Orientation o)
{
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
	actionQueue.push(GOFORWARD);
	return;
}

void Agent::goUp(Orientation o)
{
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
	actionQueue.push(GOFORWARD);
	return;
}

void Agent::goDown(Orientation o)
{
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
	actionQueue.push(GOFORWARD);
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



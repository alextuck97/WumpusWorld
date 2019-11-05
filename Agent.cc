// Agent.cc



#include "Agent.h"

using namespace std;

Location operator+(Location lhs, Location rhs)
{
	int X = lhs.X + rhs.X;
	int Y = lhs.Y + rhs.Y;
	return Location(X, Y);
}

int * binary(int n)
{
	int b = ceil(log2((float)n));
	int * binary_rep = new int[b];
	int t = 0;
	for(int i = 1 << 31; i > 0; i = i /2)
		binary_rep[t] = n & i;

	return binary_rep;
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

	if (actionQueue.empty())
	{
		updateState(percept);
		selectAction(percept);
	}
	
	action = actionQueue.front();
	actionQueue.pop();
	return action;
}



void Agent::GameOver(int score)
{

}


void Agent::selectAction(Percept &percept)
{
	if(percept.Glitter)
	{
		actionQueue.push(GRAB);
		state.hasGold = true;
		return;
	}
	//How to guide the search? 
}

void Agent::computePitProbability()
{
	for(auto f = state.frontier.begin(); f != state.frontier.end(); f++)
	{
		float p_pit_true = 0.f;
		float p_pit_false = 0.f;
		std::vector<Location> frontier_prime = state.frontier;
		auto f_prime = std::find(frontier_prime.begin(), frontier_prime.end(), *f);
		frontier_prime.erase(f_prime);

		for(int i = 0; i < powf(2.f, frontier_prime.size()); i++)
		{
			
			float num_true;
			float num_false;
			float p_frontier_prime = powf(0.2f,num_true)*pow(0.8f,num_false);

			if(true)//breezeIsConsistentWithTrue(state.breeze, ))
			{
				p_pit_true += p_frontier_prime;
			}

			if(false)//breezeIsConsistentWithFalse(state.breeze,))
			{
				p_pit_false += p_frontier_prime;
			}
		}

		p_pit_true *= 0.2f;
		p_pit_false *= 0.8f;
		p_pit_true = p_pit_true / (p_pit_true + p_pit_false);
		pit_probabilities[f->X][f->Y] = p_pit_true;
	}
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
	adj.push_back(l + Location(-1,0));
	adj.push_back(l + Location(0,1));
	adj.push_back(l + Location(1,0));
	adj.push_back(l + Location(0,-1));

	for(auto it = adj.begin(); it != adj.end(); it++)
	{
		if(it->X < 0 && it->Y < 0 && it->X >= WORLD_SIZE && it->Y >= WORLD_SIZE)
			adj.erase(*it);
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



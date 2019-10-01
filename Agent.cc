// Agent.cc

#include <iostream>
#include "Agent.h"

using namespace std;

Agent::Agent ()
{

}

Agent::~Agent ()
{
	
}



void Agent::Initialize ()
{
	state.hasGold = false;
	state.hasArrow = true;
	state.wumpusAlive = true;
	
	state.agentLocation = Location(1, 1);
	state.orientation = RIGHT;
	
	world.currentLocation = world.initialSquare;

}

Action Agent::Process (Percept& percept)
{
	if (state.previousAction == GOFORWARD && !percept.Bump) {
		world.newBoardSquare(state.orientation);
	}
	else if (percept.Bump) {
		world.currentLocation->wall = state.orientation;
	}
		

	Action action;
	if (percept.Breeze) {
		world.currentLocation->breeze = true;
	}
	if (percept.Stench) {
		world.currentLocation->stench = true;
	}
	if (percept.Glitter) {
		world.currentLocation->gold = true;
	}
	
	state.previousAction = action;
	return action;
}

void Agent::GameOver (int score)
{

}


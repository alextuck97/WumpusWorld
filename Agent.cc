// Agent.cc

#include <iostream>
#include "Agent.h"

using namespace std;

Agent::Agent ()
{

}

Agent::~Agent ()
{
	SaveWorldInformation(this->state);
}



void Agent::Initialize ()
{
	state.hasGold = false;
	state.wumpusLocation = Location(-1,-1);
	state.goldLocation = Location(-1,-1);
	state.agentLocation = Location(1, 1);
	state.worldSize = -1;
	state.orientation = RIGHT;
	state.previousAction = TURNLEFT;

	ReadWorldInformation(this->state);
	
	
	
	

}

Action Agent::Process (Percept& percept)
{
	Action action;

	//process percepts
	if(percept.Bump)
	{
		state.worldSize = (state.agentLocation.X > state.agentLocation.Y ? state.agentLocation.X : state.agentLocation.Y);
	}
	//Add a stench iff agentlocation not in stench vector
	handleStench(percept.Stench);
	
	state.wumpusLocation = locateWumpus(state.wumpusLocation);

	action = handleGlitter(percept.Glitter);

	if(state.hasGold && action != GRAB)
	{
		action = getToOrigin(state.wumpusLocation);
	}
	else
	{
		
	}
	
	
	state.previousAction = action;
	return action;
}

void Agent::GameOver (int score)
{

}


void Agent::handleStench(bool stench){
	if(stench)
	{
		std::vector<Location>::iterator it;
		bool found = false;
		for(it = state.stenches.begin(); it != state.stenches.end(); ++it){
			if(*it == state.agentLocation)
			{
				found = true;
				break;
			}
		}
		if(!found)
			state.stenches.push_back(state.agentLocation);
	}
	else
	{
		std::vector<Location>::iterator it;
		bool found = false;
		for(it = state.notStenches.begin(); it != state.notStenches.end(); ++it){
			if(*it == state.agentLocation)
			{
				found = true;
				break;
			}
		}
		if(!found)
			state.notStenches.push_back(state.agentLocation);
	}
}


Action Agent::handleGlitter(bool glitter){
	Action action;
	if(glitter)
	{
		action = GRAB;
		state.hasGold = true;
		state.goldLocation = state.agentLocation;
	}
	return action;
}

Location Agent::locateWumpus(Location wumpusLocation){

	
	if(wumpusLocation == Location(-1,-1))
	{
		if(state.stenches.size() >= 3){
			//Can locate
		}
		else if(state.stenches.size() >= 2){
			//can try
		}
		else{//No chance
			return wumpusLocation;
		}
	}
	else{
		return wumpusLocation;
	} 
}


void Agent::ReadWorldInformation(State &theState)
{
	char * s = "WorldData.txt";
	ifstream f;
	f.open(s);
	if(f.is_open())
	{
		std::string what;
		int X, Y;

		while(f >> what >> X >> Y){
			if(what.compare("GOLDLOCATION") == 0){
				theState.goldLocation.X = X;
				state.goldLocation.Y = Y;
			}
			else if(what.compare("WUMPUSLOCATION") == 0){
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
	char * s = "WorldData.txt";
	ofstream f(s, std::ios::out | std::ios::trunc);
	
	if(f.is_open())
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


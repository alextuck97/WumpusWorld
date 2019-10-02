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
	
	
	state.agentLocation = Location(1, 1);
	state.wumpusLocation = Location(-1,-1);
	state.orientation = RIGHT;
	
	

}

Action Agent::Process (Percept& percept)
{
	Action action;
	


	state.previousAction = action;
	return action;
}

void Agent::GameOver (int score)
{

}


void Agent::ReadWorldInformation(State &state)
{
	char * s = "WorldData.txt";
	ifstream f;
	f.open(s);
	if(f.is_open())
	{
		Location l;
	}
	else
	{
		printf("Error opening %s\n", s);	
	}
}

void Agent::SaveWorldInformation(State state)
{
	char * s = "WorldData.txt";
	ofstream f;
	f.open(s);
	if(f.is_open())
	{
		f << "GOLDLOCATION " << state.goldLocation.X << " " << state.goldLocation.Y << std::endl;
		f << "WUMPUSLOCATION " << state.wumpusLocation.X << " " << state.wumpusLocation.Y << std::endl;
	}
	else
	{
		printf("Error opening %s\n", s);	
	}
}


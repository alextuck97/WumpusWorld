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
	


	state.previousAction = action;
	return action;
}

void Agent::GameOver (int score)
{

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


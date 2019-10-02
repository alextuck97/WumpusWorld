// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include <fstream>
#include <string>
//#include "WorldGraph.h"


struct State {
	
		Location goldLocation;
		Location wumpusLocation;
		std::vector<Location> stenches;
		int worldSize;

		Action previousAction;
		Orientation orientation;
		Location agentLocation;
		bool hasGold;
	
};

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);


	//Agent maintains state information directly relating to itself.
	State state;

	//Functions for saving and reading world information on agent death
	//and resurrection
	void ReadWorldInformation(State &theState);
	void SaveWorldInformation(State theState);

};


#endif //AGENT_H
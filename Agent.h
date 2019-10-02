// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include <fstream>
//#include "WorldGraph.h"

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);

	//Agent maintains state information directly relating to itself.
	struct State {
	
		Location goldLocation;
		Location wumpusLocation;
		std::vector<Location> stenches;
		int worldSize = -1;

		Action previousAction;
		Orientation orientation;
		Location agentLocation;
		bool hasGold;
	}state;

	void ReadWorldInformation(State &state);
	void SaveWorldInformation(State state);

};


#endif //AGENT_H
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
		std::vector<Location> notStenches;
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

	Action getToOrigin(Location wumpusLocation);
	void handleStench(bool stench);
	Action handleGlitter(bool glitter);

	//Attempt to locate the wumpus if it is not known
	Location locateWumpus(Location wumpusLocation);
	void testWumpusLocator();
	//Functions for saving and reading world information on agent death
	//and resurrection
	void ReadWorldInformation(State &theState);
	void SaveWorldInformation(State theState);

};

//If a component of lhs - rhs is 0, they have the same x or y coord,
//implying a stench exists between them
Location operator+(Location &lhs, Location &rhs);

Location operator-(Location &lhs, Location &rhs);

#endif //AGENT_H
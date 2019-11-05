// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include <fstream>
#include <string>
#include <queue>
#include <algorithm>
#include <map>

#define WORLD_SIZE 5

struct State {
	
	Location goldLocation;
	Location wumpusLocation;
	std::vector<Location> stenches;

	std::vector<Location> frontier;
	std::vector<Location> breeze;
	std::vector<Location> known_pits;
	std::vector<Location> known_not_pits;

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

	State state;
	
	std::queue<Action> actionQueue;
	
	Action selectAction(Percept &percept);

	void computePitProbability(); 

	void updateState(Percept &percept);

	std::vector<Location> getAdjacentLocations(Location l);
	void goLeft(Orientation o);
	void goRight(Orientation o);
	void goUp(Orientation o);
	void goDown(Orientation o);

	void updateAgentLocation(Orientation o);
};

//If a component of lhs - rhs is 0, they have the same x or y coord,
//implying a stench exists between them
Location operator+(Location lhs, Location rhs);

Location operator-(Location &lhs, Location &rhs);



#endif //AGENT_H
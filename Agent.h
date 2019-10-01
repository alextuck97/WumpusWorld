// Agent.h

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include "WorldGraph.h"

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
		bool hasArrow;
		bool wumpusAlive;
		bool hasGold;
		Orientation orientation;
		Location location;
		Action previousAction;
	}state;

	WorldGraph world;

};

#endif // AGENT_H

// Agent.cc

#include <iostream>
#include "Agent.h"

using namespace std;

Location operator+(Location &lhs, Location &rhs){
	int X = lhs.X + rhs.X;
	int Y = lhs.Y + rhs.Y;
	return Location(X,Y);
}

//If a component of lhs - rhs is 0, they have the same x or y coord,
//implying a stench exists between them
Location operator-(Location &lhs, Location &rhs){
	int X = lhs.X - rhs.X;
	int Y = lhs.Y - rhs.Y;
	return Location(X,Y);
}

Agent::Agent ()
{

}

Agent::~Agent ()
{
	SaveWorldInformation(this->state);
}



void Agent::Initialize ()
{

	testWumpusLocator();
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

//Push stench back into stench vector if it hasn't been discovered
//Push normal square into nonstench vector if it hasn't been discovered
void Agent::handleStench(bool stench){
	if(stench)
	{
		bool found = false;
		for(int i = 0; i < state.stenches.size(); i++){
			if(state.stenches.at(i) == state.agentLocation)
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
		for(int i = 0; i < state.notStenches.size(); i++){
			if(state.notStenches.at(i) == state.agentLocation)
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
			Location difference0 = state.stenches.at(0) - state.stenches.at(1);
			Location difference1 = state.stenches.at(0) - state.stenches.at(2);
			
			Location w;
			//Because the X or Y component is the same, the average will give 
			//the coordinate between the location component that was not equal,
			//the wumpus location
			if(difference0.X == 0 || difference0.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(1);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}
			else if(difference1.X == 0 || difference1.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(2);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}

			return w;
		}
		else if(state.stenches.size() >= 2){
			Location difference0 = state.stenches.at(0) - state.stenches.at(1);
			Location w;
			//Wumpus directly between stenches
			if(difference0.X == 0 || difference0.Y == 0)
			{
				w = state.stenches.at(0) + state.stenches.at(1);
				w.X = w.X / 2;
				w.Y = w.Y / 2;
			}
			//Stenches are diagonal
			else 
			{
				Location s0 = state.stenches.at(0);
				Location s1 = state.stenches.at(1);
				
				for(int i = 0; i < state.notStenches.size(); i++)
				{
					Location notS = state.notStenches.at(i);
					if(Adjacent(s0, notS) && Adjacent(s1, notS))
					{
						Location dir0 = s0 - notS;
						Location dir1 = s1 - notS;
						w = notS + dir0 + dir1;
					}
				}
			}
			return w;
		}
		else{//One stench, no chance
			return wumpusLocation;
		}
	}
	//Already know location
	else{
		return wumpusLocation;
	} 
}

void Agent::testWumpusLocator()
{
	//Two stenches with block in between case
	state.stenches.clear();
	state.stenches.push_back(Location(2,1));
	state.stenches.push_back(Location(2,3));
	Location loc = locateWumpus(Location(-1,-1));

	printf("Size: %d S1: %d, %d S2: %d, %d W: %d, %d\n", 2, 2,1,2,3,loc.X, loc.Y);

	//Three stenches case
	state.stenches.clear();
	state.stenches.push_back(Location(2,1));
	state.stenches.push_back(Location(4,1));
	state.stenches.push_back(Location(3,2));
	loc = locateWumpus(Location(-1,-1));
	printf("Size: %d S1: %d, %d S2: %d, %d S3: %d, %d W: %d, %d\n", 3, 2,1,4,1,3,2, loc.X, loc.Y);

	//Two stenches with an adjacent nonStench block case
	state.stenches.clear();
	state.stenches.push_back(Location(3,2));
	state.stenches.push_back(Location(2,1));
	state.notStenches.push_back(Location(2,2));
	loc = locateWumpus(Location(-1,-1));
	printf("Size: %d S1: %d, %d S2: %d, %d NS: %d, %d W: %d, %d\n", 2, 3,2,2,1,2,2, loc.X, loc.Y);

	//Two stenches with a non-adjacent nonStench block case
	state.stenches.clear();
	state.stenches.push_back(Location(3,2));
	state.stenches.push_back(Location(2,1));
	state.notStenches.push_back(Location(2,4));
	loc = locateWumpus(Location(-1,-1));
	printf("Size: %d S1: %d, %d S2: %d, %d NS: %d, %d W: %d, %d\n", 2, 3,2,2,1,2,4, loc.X, loc.Y);
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


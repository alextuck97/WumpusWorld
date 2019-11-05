#include "Test.h"


bool TestGetAdjacentLocations(Agent &a, Location l)
{
    std::vector adj = a.getAdjacentLocations(l);
    std::vector<Location> expected;
    expected.push_back(Location(l.X, l.Y - 1));
    expected.push_back(Location(l.X, l.Y + 1));
    expected.push_back(Location(l.X - 1, l.Y));
    expected.push_back(Location(l.X + 1, l.Y));
    for(auto it = adj.begin(); it != adj.end(); it++)
    {
        
    }
}

bool TestUpdateState(Agent &a)
{

}



void RunTests()
{
    Agent a;
}
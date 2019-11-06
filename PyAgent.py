# PyAgent.py

import Action as A
import Orientation as O
from queue import Queue

WORLD_SIZE = 5

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3

class Agent:
   
    def __init__(self):
        self.action_queue = Queue()
        self.has_gold = False
        self.known = {0:[],1:[]}#List of not pits (0) and pits (1)
        self.breezes = []
        self.frontier = []
        self.stenches = []
        self.orientation = O.RIGHT
        self.x_loc = 1
        self.y_loc = 1
        self.previous_action = A.CLIMB
        self.pit_probabilities = {}

    def go_left(self):
        self.orientation = O.LEFT
        if self.state.orientation == O.UP:
            self.action_queue.put(A.TURNLEFT)
        elif self.state.orientation == O.DOWN:
            self.action_queue.put(A.TURNRIGHT)
        elif self.state.orientation == O.RIGHT:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)
    
    def go_right(self):
        self.orientation = O.RIGHT
        if self.state.orientation == O.UP:
            self.action_queue.put(A.TURNRIGHT)
        elif self.state.orientation == O.DOWN:
            self.action_queue.put(A.TURNLEFT)
        elif self.state.orientation == O.LEFT:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)

    def go_up(self):
        self.orientation = O.UP
        if self.state.orientation == O.RIGHT:
            self.action_queue.put(A.TURNLEFT)
        elif self.state.orientation == O.LEFT:
            self.action_queue.put(A.TURNRIGHT)
        elif self.state.orientation == O.DOWN:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)

    def go_down(self):
        self.orientation = O.DOWN
        if self.state.orientation == O.RIGHT:
            self.action_queue.put(A.TURNRIGHT)
        elif self.state.orientation == O.LEFT:
            self.action_queue.put(A.TURNLEFT)
        elif self.state.orientation == O.DOWN:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)

    def update_state(self, stench, breeze, glitter, bump, scream):
        
        location = (self.x_loc, self.y_loc)
        if location not in self.known[0] and location not in self.known[1]:
            self.known[0].append(location)
        
        if stench and location not in self.stenches:
            self.stenches.append(location)
        
        if breeze and location not in self.breezes:
            self.breezes.append(location)

        if location in self.frontier:
            self.frontier.remove(location)

        adjacent_to_me = [(location[0], location[1] - 1 ),
                            (location[0],location[1] + 1),
                            (location[0] - 1, location[1]),
                            (location[0] + 1, location[1])]
        
        for l in adjacent_to_me:
            #Check if the adjacent square is in bounds
            if l[0] >= 1 and l[0] <= WORLD_SIZE and l[1] >= 1 and l[1] <= WORLD_SIZE:
                #Add it to the frontier if it is not known and not in the frontier
                if l not in self.frontier and l not in self.known[0] and l not in self.known[1]:
                    self.frontier.append(l)
            
    def calculate_pits(self):
        pass

    def locate_wumpus(self):
        pass

    def pick_action(self, glitter):
        if glitter:
            self.action_queue.put(A.GRAB)
        elif self.has_gold:
            #Get to the exit
            pass
        else:
            #Explore
            actions = {}

    def update_location(self):
        if self.orientation == O.LEFT:
            self.x_loc -= 1
        elif self.orientation == O.RIGHT:
            self.x_loc += 1
        elif self.orientation == O.UP:
            self.y_loc += 1
        elif self.orientation[1] == O.DOWN:
            self.y_loc -= 1


a = Agent()


def PyAgent_Constructor ():
    print("PyAgent_Constructor")

def PyAgent_Destructor ():
    print("PyAgent_Destructor")

def PyAgent_Initialize ():
    print("PyAgent_Initialize")

def PyAgent_Process (stench,breeze,glitter,bump,scream):
    
    if a.previous_action == A.CLIMB:#This means the agent is brand new in the world
        a.update_state(stench,breeze,glitter,bump,scream)

    if a.previous_action == A.GOFORWARD and not bump:
        a.update_location()
        a.update_state(stench, breeze, glitter, bump, scream)

    if a.action_queue.empty():
        a.update_state(stench, breeze, glitter, bump, scream)
    else:
        action = a.action_queue.get()

    return action

def PyAgent_GameOver (score):
    print("PyAgent_GameOver: score = " + str(score))



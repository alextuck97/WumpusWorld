# PyAgent.py
import random 
import Action as A
import Orientation as O
from queue import Queue

WORLD_SIZE = 5

LEFT = 0
RIGHT = 1
UP = 2
DOWN = 3


class GraphNode:
    def __init__(self, value, parent):
        self.value = value
        self.parent = parent
        self.child = []

    def add_child(self, child):
        self.child.append(child)

class Agent:
   
    def __init__(self):
        self.action_queue = Queue()
        self.has_gold = False
        self.gold_location = (-1,-1)
        self.target_location = (-1,-1)
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
        if self.orientation == O.UP:
            self.action_queue.put(A.TURNLEFT)
        elif self.orientation == O.DOWN:
            self.action_queue.put(A.TURNRIGHT)
        elif self.orientation == O.RIGHT:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)
    
    def go_right(self):
        self.orientation = O.RIGHT
        if self.orientation == O.UP:
            self.action_queue.put(A.TURNRIGHT)
        elif self.orientation == O.DOWN:
            self.action_queue.put(A.TURNLEFT)
        elif self.orientation == O.LEFT:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)

    def go_up(self):
        self.orientation = O.UP
        if self.orientation == O.RIGHT:
            self.action_queue.put(A.TURNLEFT)
        elif self.orientation == O.LEFT:
            self.action_queue.put(A.TURNRIGHT)
        elif self.orientation == O.DOWN:
            self.action_queue.put(A.TURNRIGHT)
            self.action_queue.put(A.TURNRIGHT)
        self.action_queue.put(A.GOFORWARD)

    def go_down(self):
        self.orientation = O.DOWN
        if self.orientation == O.RIGHT:
            self.action_queue.put(A.TURNRIGHT)
        elif self.orientation == O.LEFT:
            self.action_queue.put(A.TURNLEFT)
        elif self.orientation == O.DOWN:
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

        adjacent_to_me = self.get_adjacent(location)
        
        for l in adjacent_to_me:
            #Check if the adjacent square is in bounds
            if l[0] >= 1 and l[0] <= WORLD_SIZE and l[1] >= 1 and l[1] <= WORLD_SIZE:
                #Add it to the frontier if it is not known and not in the frontier
                if l not in self.frontier and l not in self.known[0] and l not in self.known[1]:
                    self.frontier.append(l)
            
    def calculate_pits(self, frontier, breeze, known):
        '''
        Based off of frontier, breeze, and known, calculate
        the probability of pits
        '''
        for p in self.known[0]:
            self.pit_probabilities[p] = 0
        
        for p in self.known[1]:
            self.pit_probabilities[p] = 1
        
        for f in frontier:
            p_true = 0
            p_false = 0
            frontier_p = frontier.copy()
            frontier_p.remove(f)

            for C in range(2**len(frontier_p)):
                #Use binary representation to get pit true/false combinations
                bin_c = bin(C)[2:]
                app_to_c = len(bin(2**len(frontier_p) - 1)[2:]) - len(bin_c)
                for _ in range(app_to_c):
                    bin_c = "0" + bin_c
                
                num_trues = 0 
                num_false = 0
                for i in range(len(bin_c)):
                    if bin_c[i] == "1":
                        num_trues += 1
                    elif bin_c[i] == "0":
                        num_false += 1

                p_frontier_p = (0.2**num_trues)*(0.8**num_false)

                #Is breeze conistent when pit f is true?
                if self.is_breeze_consistent(breeze, frontier_p, bin_c, f):
                    p_true += p_frontier_p
                #Is breeze consistent when pit f is false?
                if self.is_breeze_consistent(breeze, frontier_p, bin_c):
                    p_false += p_frontier_p
            
            p_true *= 0.2
            p_false *= 0.8
            self.pit_probabilities[f] = float('%.2f'%(p_true / (p_true + p_false)))

    def is_breeze_consistent(self, breeze, frontier_prime, C,testing_pit = None):
        '''
        Test is the breeze is consistent with the combination of pits
        that are true in this combination. 

        breeze: list of breezes

        frontier_prime: frontier minus the pit we currently calculating
        probability for

        C: binary combination of the pits that are true in this combination
        
        testing_pit: the pit we are calculating probability for
        '''
        if testing_pit is not None:
            pits = [testing_pit]
        else:
            pits = []
        #Make list of which pits are true for this combination
        for i in range(len(frontier_prime)):
            if C[i] == '1':
                pits.append(frontier_prime[i])

        #For each pit
        for p in pits:
            breeze_consistent = False
            #For each breeze
            for b in breeze:
                #Check if a breeze is adjacent to the pit. If yes, breezes are consistent with pit location
                c = tuple(i1 - i2 for i1, i2 in zip(p,b))
                if (abs(c[0]) == 1 and abs(c[1]) == 0) or (abs(c[0]) == 0 and abs(c[1]) == 1):
                    breeze_consistent = True
                    break
            #If breeze_consistent was not set to True, there is at least one pit breeze information
            #is not consistent with
            if breeze_consistent == False:
                return False
        
        #For each breeze
        for b in breeze:
            pit_consistent = False
            #For each pit
            for p in pits:
                #Check if a pit is adjacent to the breeze. If yes, pit are consistent with breeze location
                c = tuple(i1 - i2 for i1, i2 in zip(p,b))
                if (abs(c[0]) == 1 and abs(c[1]) == 0) or (abs(c[0]) == 0 and abs(c[1]) == 1):
                    pit_consistent = True
                    break
            #If pit_consistent was not set to True, there is at least one pit not 
            #consistent with breeze information
            if pit_consistent == False:
                return False
        
        return True

    def get_adjacent(self, location):
        return [(location[0], location[1] - 1 ),
                    (location[0],location[1] + 1),
                    (location[0] - 1, location[1]),
                    (location[0] + 1, location[1])]

    def locate_wumpus(self):
        pass

    def pick_action(self, glitter):
        '''
        Considers the current state, and pushes actions
        to the queue
        '''
        if glitter:
            self.action_queue.put(A.GRAB)
        elif self.has_gold:
            #Get to the exit
            pass
        elif self.gold_location != (-1,-1):
            #Get to gold
            pass
        else:
            self.target_location = self.get_new_target_to_explore()
            self.get_to_target(self.target_location)


    def get_new_target_to_explore(self):
        '''
        Find the frontier location with the least risk 
        If multiple equally least risky frontier points, pick randomly
        '''
        min_risk = 1000000
        candidate_locations = []
        for f in self.frontier:
            if self.pit_probabilities[f] == min_risk:
                candidate_locations.append(f)
            elif self.pit_probabilities[f] < min_risk:
                min_risk = self.pit_probabilities[f]
                candidate_locations.clear()
                candidate_locations.append(f)

        r = random.randint(0,len(candidate_locations)-1)
        
        return candidate_locations[r]



    def get_to_target(self, target_location):
        '''
        Push actions to the queue to get the agent to the target
        Assumes target is on frontier or known
        '''
        actions = {LEFT:self.go_left, RIGHT:self.go_right,
            UP:self.go_up, DOWN:self.go_down}
        directions = {(0,-1):DOWN,(0,1):UP,(1,0):RIGHT,(-1,0):LEFT}
        location = (self.x_loc,self.y_loc)

        #BFS
        g = GraphNode(location,None)

        frontier = Queue()
        frontier.put(g)
        explored = set()
        found = False
        while not frontier.empty() or found:
            f = frontier.get()
            explored.add(f.location)

            adj = self.get_adjacent(f.value)

            for a in adj:
                #if a has not been explored and a is not a pit
                if a not in explored and self.pit_probabilities[a] == 0:
                    a_node = GraphNode(a,f)
                    f.add_child(a_node)
                    if a_node.value == target_location:
                        f = a_node
                        found = True
                    else:
                        frontier.put(a_node)
        
        move_list = []
        while f.parent is not None:
            #Create a list of moves the agent must execute
            c = tuple(i1 - i2 for i1, i2 in zip(f.parent.value,f.value))
            move_list.append(directions[c])
        
        move_list.reverse()
        for m in move_list:
            m()


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
        a.pick_action(glitter)
    
    action = a.action_queue.get()

    return action

def PyAgent_GameOver (score):
    print("PyAgent_GameOver: score = " + str(score))


if __name__ == "__main__":
    a = Agent()
    frontier = [(3,1),(1,3),(2,2)]
    breezes = [(1,2),(2,1)]
    expected = {(3,1):0.31,(2,2):0.86,(1,3):0.31}
    a.calculate_pits(frontier, breezes, [])
    print(a.pit_probabilities)
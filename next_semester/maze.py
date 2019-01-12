from node import *
import numpy as np
import csv
import pandas
from enum import IntEnum
import math

class Action(IntEnum):
    ADVANCE    = 1
    U_TURN     = 2
    TURN_RIGHT = 3
    TURN_LEFT  = 4
    HALT       = 5

class Maze:
    def __init__(self, filepath):
        """
        read file and implement a data structure you like
        """
        self.raw_data = pandas.read_csv(filepath).values

########################### TODO ###########################

        self.nodes = []
        self.nd_dict = dict() # key: index, value: the correspond node
        self.explored = set()

        # for dt in self.raw_data:
        #     nd = node.Node(dt[0])
        #     for i in range(1,5):
        #         if not math.isnan(dt[i]):
        #             nd.setSuccessor(int(dt[i]))

        print("Creating the maze............")
        for dt in self.raw_data:
            index = int(dt[0])
            node_tmp = Node(index)
            for successors_index in range(1,5):
                if not math.isnan(dt[successors_index]):
                    if not math.isnan(dt[successors_index+4]):
                        node_tmp.setSuccessor(int(dt[successors_index]), successors_index, int(dt[successors_index+4]))
                    else:
                        node_tmp.setSuccessor(int(dt[successors_index]), successors_index)
            self.nodes.append(node_tmp)
            # Add nd_dictionary by {key(index) : value(node)}
            self.nd_dict[index] = node_tmp
        print("Maze creation finished!")
        # For debug !
        # for node in self.nodes:
        #     print("Index: ", node.index, "\n", node.Successors)

#############################################################

    
#     def shortestPath(self, nd_from, nd_to):
#         """ 
#         return a path (sequence of nodes) from the current node to the nearest unexplored deadend 
#         e.g.
#             1 -- 2 -- 3     
#                  |    |  ->  shortestPath(1,4) returns [1,2,4]
#                  4 -- 5
#         """

# ########################### TODO ###########################

#         queue = [nd_from]
#         explored = [-1] * (len(self.nd_dict) + 1)
#         explored[nd_from] = 0
#         while len(queue) != 0:
#             for succ in self.nd_dict[queue[0]].getSuccessors():
#                 if succ == nd_to:
#                     ret = [nd_to]
#                     curr_nd = queue[0]
#                     while explored[curr_nd] != -1:
#                         ret.append(curr_nd)
#                         curr_nd = explored[curr_nd]
#                     ret.reverse()
#                     return ret

#                 if explored[succ] == -1:
#                     explored[succ] = queue[0]
#                     queue.append(succ)

#             queue.pop(0)

# ##############################################################
#         return []
    
    def getStartPoint(self):
        if (len(self.nd_dict) < 2):
            print ("Error: the start point is not included.")
            return 0
        return self.nd_dict[1]

    def getNodeDict(self):
        return self.nd_dict
    
    def BFS(self, nd):
        """ return a sequence of nodes from the node to the nearest unexplored deadend"""
        #TODO: design your data structure here for your algorithm
        queue = [nd]
        # explored set
        ndList = []
        transitionTable = dict()
        explored = set()
        # main loop
        while True:
            if not queue:  # if Q is empty
                print('End point is not found!')
                break
            # u : the last element of queue
            u = queue.pop()
            # print(self.nd_dict[u.getSuccessors()[0][0]] in explored)
            if (len(u.getSuccessors()) is 1) and (self.nd_dict[u.getSuccessors()[0][0]] in explored) :  # check if u is the end node
                nd_to = u
                break  # return the transition table
            explored.add(u)  # add u to explored set
            for v in u.getSuccessors():  # loop over all adjacency of u
                node = self.nd_dict[v[0]]
                if (node not in queue) and (node not in explored):
                    # Add to the first elemenet in queue
                    queue.insert(0, node)  # add v to Q
                    transitionTable[node] = u
        # from transition table (transitionTable) to answer (ndList)
        now_nd = nd_to
        ndList.insert(0, now_nd)
        while now_nd is not nd:
            now_nd = transitionTable[now_nd]
            ndList.insert(0, now_nd)  # insert to the front end, then no need to reverse
        # what I write end ------
        return ndList 

    def BFS_2(self, nd_from, nd_to):
        """ return a sequence of nodes of the shortest path"""
        # TODO: similar to BFS but fixed start point and end point
        ndList = []
        # what I write begin ------
        # initialize
        queue = [nd_from]  # Q: contains only start node initially
        # print(nd_from)
        transitionTable = dict()
        explored = set()
        # main loop
        while True:
            if not queue:  # if Q is empty
                print('End point is not found!')
                break
            # u : the last element of queue
            u = queue.pop()
            if u is nd_to:  # check if u is the end node
                break  # return the transition table
            explored.add(u)  # add u to explored set
            for v in u.getSuccessors():  # loop over all adjacency of u
                node = self.nd_dict[v[0]]
                if (node not in queue) and (node not in explored):
                    # Add to the first elemenet in queue
                    queue.insert(0, node)  # add v to Q
                    transitionTable[node] = u
        # from transition table (transitionTable) to answer (ndList)
        now_nd = nd_to
        ndList.insert(0, now_nd)
        while now_nd is not nd_from:
            now_nd = transitionTable[now_nd]
            ndList.insert(0, now_nd)  # insert to the front end, then no need to reverse
        # what I write end ------
        for i in ndList:
            print(i.getIndex())
        return ndList 

    def getAction(self, car_dir, nd_from, nd_to):
        """ return an action and the next direction of the car """
        print(nd_from.getIndex())
        print(nd_to.getIndex())
        print(nd_from.getSuccessors())
        print(nd_to.getSuccessors())
        if nd_from.isSuccessor(nd_to.getIndex()):
            nd_dir = nd_from.getDirection(nd_to.getIndex())
            #TODO: Return the action based on the current car direction and the direction to next node
            if car_dir not in range(1,5):
                raise Exception("car_dir invalid ERROR!")
            if nd_dir == car_dir:
                return Action.ADVANCE
            if ((car_dir == 1) and (nd_dir == 4)) or\
                ((car_dir == 4) and (nd_dir == 2)) or\
                ((car_dir == 2) and (nd_dir == 3)) or\
                ((car_dir == 3) and (nd_dir == 1)):
                return Action.TURN_RIGHT
            if ((car_dir == 1) and (nd_dir == 3)) or\
                ((car_dir == 3) and (nd_dir == 2)) or\
                ((car_dir == 2) and (nd_dir == 4)) or\
                ((car_dir == 4) and (nd_dir == 1)):
                return Action.TURN_LEFT
            if ((car_dir == 1) and (nd_dir == 2)) or\
                ((car_dir == 3) and (nd_dir == 4)) or\
                ((car_dir == 2) and (nd_dir == 1)) or\
                ((car_dir == 4) and (nd_dir == 3)):
                return Action.U_TURN
            print("Error: Failed to get the action")
            return Action.HALT
        else:
            print("Error: Node(",nd_to.getIndex(),") is not the Successor of Node(",nd_from.getIndex(),")")
            return 0

    def strategy(self, nd):
        return self.BFS(nd)

    def strategy_2(self, nd_from, nd_to):
        return self.BFS_2(nd_from, nd_to)
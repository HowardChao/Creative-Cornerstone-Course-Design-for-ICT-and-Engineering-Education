from enum import IntEnum

class Direction(IntEnum):
    NORTH = 1
    SOUTH = 2
    WEST  = 3
    EAST  = 4

# TODO: Define your own Node class!

# Suggest methods: 
#  1. getIndex(): return node index.
#  2. getSuccessors(): return all successors of the node.
#  3. setSuccessor(): set the successors of the given node.
#  4. Other helpful methods!

class Node:
    def __init__(self, index=0):
        # The index of the node
        self.index = index
        # The successor node of the current node
        self.Successors = []


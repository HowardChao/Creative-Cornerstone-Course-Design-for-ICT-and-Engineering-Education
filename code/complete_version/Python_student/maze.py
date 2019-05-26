from node import *
import numpy as np
import csv
import pandas
from enum import IntEnum
import math


class Action(IntEnum):
    ADVANCE = 1
    U_TURN = 2
    TURN_RIGHT = 3
    TURN_LEFT = 4
    HALT = 5


# TODO: Define your own Maze class!
# Suggest methods: 
#  1. getStartPoint(): Get the start point of the maze.
#  2. getNodeDict(): Get the whole node information.
#  3. strategy_1(): The strategy for Game 1 (rule 1).
#  4. strategy_2(): The strategy for Game 2 (rule 2).
#  5. Other helpful methods!
class Maze:
    def __init__(self, filepath):
        # Read in your data
        self.raw_data = pandas.read_csv(filepath).values
        self.nodes = []
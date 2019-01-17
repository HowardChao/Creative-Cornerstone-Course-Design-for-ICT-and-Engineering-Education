from node import *
import maze as mz
import score
import interface
import time

import numpy as np
import pandas
import time
import sys
import os


def main():
    maze = mz.Maze("data/maze_test.csv")
    point = score.Scoreboard("data/UID.csv")
    interf = interface.interface()         # the part of calling interface.py was commented out.
    # TODO : Initialize necessary variables

    if (sys.argv[1] == '0'):
        print("Mode 0: for treasure-hunting with rule 1")
        # TODO : for treasure-hunting with rule 1, which encourages you to hunt as many scores as possible

    elif (sys.argv[1] == '1'):
        print("Mode 1: for treasure-hunting with rule 2")
        # TODO : for treasure-hunting with rule 2, which requires you to hunt as many specified treasures as possible

    elif (sys.argv[1] == '2'):
        print("Mode 2: Self-testing mode.")
        # TODO: You can write your code to test specific function.


if __name__ == '__main__':
    main()

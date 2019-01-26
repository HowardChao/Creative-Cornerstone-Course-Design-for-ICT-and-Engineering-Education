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
    next_nd = maze.getStartPoint()
    node_dict = maze.getNodeDict()
    car_dir = Direction.SOUTH
    point = score.Scoreboard("data/UID.csv")
    interf = interface.interface()         # the part of calling interface.py was commented out.

    # Mode 0: for treasure-hunting with rule 1, which encourages you to hunt as many scores as possible.
    if (sys.argv[1] == '0'):
        print("Mode 0: for treasure-hunting with rule 1, which encourages you to hunt as many scores as possible")
        while (1):
            # ================================================
            # Basically, you will get a list of nodes and corresponding UID strings after the end of algorithm.
            # The function add_UID() would convert the UID string score and add it to the total score.
            # In the sample code, we call this function after getting the returned list.
            # You may place it to other places, just make sure that all the UID strings you get would be converted.
            # ================================================
            ndList = []
            deadend_node_num = 0
            for node in maze.nodes:
                if len(node.getSuccessors()) == 1:
                    deadend_node_num += 1
            start_nd = next_nd
            for i in range(1, deadend_node_num + 1):
                BFS_list = maze.strategy(start_nd)
                start_nd = BFS_list[-1]
                if i == deadend_node_num:
                    ndList = ndList + BFS_list
                else:
                    ndList = ndList + BFS_list[:-1]
                print("The route to deadend {}: {}".format(i, [j.getIndex() for j in BFS_list]))

            # Check the result for the whole BFS!
            print("The whole BFS route:", [node.getIndex() for node in ndList])

            count = 0
            for i in range(1, len(ndList)):
                current_node = ndList[i - 1]
                next_node = ndList[i]
                print("The coming node: Node", current_node.getIndex())
                print("The next going node: Node", next_node.getIndex())

                # current car position + current node + next node => action + new car direction
                print("Current car direction:", car_dir)
                action, car_dir = maze.getAction(car_dir, current_node, next_node)
                print("Updated car direction:", car_dir)

                # Wait until the BT says the car reaches a node
                while (1):
                    python_get_information = interf.ser.SerialReadString()
                    print(python_get_information is 'N')
                    if python_get_information is 'N':
                        count = count + 1
                        print("The car see a node!\n")
                        break

                # Tell BT to send the action back to Arduino
                print("Get action:", action)
                interf.send_action(action)
                # TODO: get UID under the node.
            print("Get action: ", mz.Action.HALT)
            interf.send_action(mz.Action.HALT)
            break

    # Mode 1: for treasure-hunting with rule 2, which requires you to hunt as many specified treasures as possible.
    elif (sys.argv[1] == '1'):
        print("Mode 1: for treasure-hunting with rule 2, which requires you to hunt as many specified treasures as possible.")
        while (1):
            nd = int(input("destination: "))
            if (nd == 0):
                print("end process")
                print('')
                break
            try:
                nd = node_dict[nd]
            except:
                print("Your input is not a valid node!")
                raise IndexError("No node!")
            ndList = maze.strategy_2(next_nd, nd)

            count = 0
            for i in range(1, len(ndList)):
                current_node = ndList[i - 1]
                next_node = ndList[i]

                # current car position + current node + next node => action + new car direction
                print("Current car direction:", car_dir)
                action, car_dir = maze.getAction(car_dir, current_node, next_node)
                print("Updated car direction:", car_dir)

                # Wait until the BT says the car reaches a node
                while (1):
                    python_get_information = interf.ser.SerialReadString()
                    print(python_get_information)
                    if python_get_information is 'N':
                        count = count + 1
                        print(python_get_information)
                        print("The car see a node!\n")
                        break

                # Tell BT to send the action back to Arduino
                print("Get action:", action)
                interf.send_action(action)
            print("Get action: ", mz.Action.HALT)
            interf.send_action(mz.Action.HALT)
            break

    # Mode 2: Self-testing mode.
    elif (sys.argv[1] == '2'):
        print("Mode 2: Self-testing mode.")
        # TODO: You can write your code to test specific function.
        while (1):
            state_cmd = input("Please enter a mode command: ")
            interf.ser.SerialWrite(state_cmd)

if __name__ == '__main__':
    main()

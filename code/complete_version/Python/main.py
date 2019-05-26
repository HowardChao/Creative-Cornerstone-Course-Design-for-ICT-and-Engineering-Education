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
import requests

def main():
    # Initialization
    maze = mz.Maze("data/maze_small.csv")
    next_nd = maze.getStartPoint()
    node_dict = maze.getNodeDict()
    car_dir = Direction.SOUTH
    point = score.Scoreboard("data/UID.csv")
    interf = interface.interface()         # the part of calling interface.py was commented out.
    team_name = "Team_1"
    # Mode 0: for treasure-hunting with rule 1
    if (sys.argv[1] == '0'):

        # When you start the game, call the following judge code.
        ######### Do not change #########



        ### Add Here !!!
        ########## Juge code ############
        point.start_Judging()
        #################################










        print("Mode 0: Game Start!")
        while (1):
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
                # Node Detecting
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
                # RFID receiving
                (read_UID, waiting) = interf.ser.SerialReadByte()
                while read_UID == "Not receive" and waiting == 0:
                    (read_UID, waiting) = interf.ser.SerialReadByte()
                while waiting < 4:
                    (read_UID_tmp, waiting_tmp) = interf.ser.SerialReadByte()
                    if waiting_tmp != 0:
                        waiting = waiting + waiting_tmp
                        read_UID = read_UID + read_UID_tmp
                print("read_UID: ", read_UID, "  waiting: ", waiting)
                print("***** waiting: ", waiting)
                if read_UID != "Not receive" and waiting == 4:
                    print("RFID ID: ", read_UID)
                    point.add_UID(read_UID)
                current_score = point.getCurrentScore()

                # When you get an UID, please call the following judge code.
                ######### Do not change #########



                ########## Juge code ############
                # current_score = point.getCurrentScore()

                #################################






            print("Get action: ", mz.Action.HALT)
            interf.send_action(mz.Action.HALT)
            break

    # Mode 1: for treasure-hunting with rule 2
    elif (sys.argv[1] == '1'):
        print("Mode 1: Game Start!")
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


    # Mode 2: Self-testing mode.
    elif (sys.argv[1] == '2'):
        print("Mode 2: Self-testing mode.")
        # TODO: You can write your code to test specific function.
        while (1):
            state_cmd = input("Please enter a mode command: ")
            interf.ser.SerialWrite(state_cmd)
            # read_UID = interf.ser.SerialReadByte()
            # print(read_UID)

if __name__ == '__main__':
    main()

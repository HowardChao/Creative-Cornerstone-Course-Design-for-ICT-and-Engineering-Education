from node import *
import maze as mz
import score
import interface
# import time

# import numpy as np
# import pandas
# import time
import sys
# import os

def main():
    maze = mz.Maze("data/maze_test.csv")
    next_nd = maze.getStartPoint()
    node_dict = maze.getNodeDict()
    car_dir = Direction.SOUTH
    # point = score.Scoreboard("data/UID.csv")
    interf = interface.interface()         # the part of calling interface.py was commented out.

    # Mode 0: for treasure-hunting with rule 1, which encourages you to hunt as many scores as possible.
    if sys.argv[1] == '0':
        print("Mode 0: for treasure-hunting with rule 1, which encourages you to hunt as many scores as possible.")
        while True:
            # TODO: Implement your algorithm here and return the UID for evaluation function
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
            # BFS_list = []
            for i in range(1, deadend_node_num+1):
                BFS_list_run = maze.strategy(start_nd)
                BFS_list = BFS_list_run
                start_nd = BFS_list[-1]
                if i == deadend_node_num:
                    ndList = ndList + BFS_list
                else:
                    ndList = ndList + BFS_list[:-1]
            
            # Print ndList to check the result for the whole BFS.
            print("The whole BFS route: ", [node.getIndex() for node in ndList])

            state_cmd = input("Please enter a mode command: ")
            interf.ser.SerialWrite(state_cmd)
            # Testing encounter the node !!
            count = 0

            for i in range(1, len(ndList)):
                current_node = ndList[i-1]
                next_node = ndList[i]
                action = maze.getAction(car_dir, current_node, next_node)
                # current car position + to node => get action
                # print("Get Action: ", action, "\n")
                print("Current Car direction: ", car_dir)
                if action == mz.Action.ADVANCE:
                    car_dir = car_dir
                elif action == mz.Action.U_TURN:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.NORTH
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.WEST
                elif action == mz.Action.TURN_RIGHT:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.WEST
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.NORTH
                elif action == mz.Action.TURN_LEFT:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.WEST
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.NORTH
                # When car arrive to a node !!!
                while True:
                    python_get_information = interf.ser.SerialReadString()
                    print(python_get_information is 'N')
                    if python_get_information is 'N':
                        count = count + 1
                        print("Get to a node!!\n")
                        break
                # python_get_information = interf.ser.SerialReadString()
                # while python_get_information is not 'N':
                #     python_get_information = interf.ser.SerialReadString()
                # print("Get to a node!!")
                # while(1):
                #     python_get_information = interf.ser.SerialReadString()
                #     if python_get_information is 'N':
                #         print(python_get_information)
                #         print("Get to a node!!\n")
                #         break

                # Send the state to Arduino
                print("Get action: ", action)
                interf.send_action(action)
                # node = 0
                # get_UID = "just a test"
                # point.add_UID(get_UID)
            print("Get action: ", mz.Action.HALT)
            interf.send_action(mz.Action.HALT)
            break

    # Mode 1: for treasure-hunting with rule 2, which requires you to hunt as many specified treasures as possible.
    elif (sys.argv[1] == '1'):
        print("Mode 1: for treasure-hunting with rule 2, which requires you to hunt as many specified treasures as possible.")
        while True:
            #TODO: Implement your algorithm here and return the UID for evaluation function
            nd = int(input("destination: "))
            if(nd == 0):
            	print("end process")
            	print('')
            	break
            try:
                nd = node_dict[nd]
            except:
                print("Your input is not a valid node !!")
                raise IndexError("No node!")
            # print(nd)
            # print(next_nd)
            ndList = maze.strategy_2(next_nd,nd)
            # Testing for getting into a node !!
            state_cmd = input("Please enter a mode command: ")
            interf.ser.SerialWrite(state_cmd)
            state_cmd = input("Please enter a mode command: ")
            interf.ser.SerialWrite(state_cmd)
            # Testing encounter the node !!
            count = 0

            for i in range(1, len(ndList)):
                current_node = ndList[i-1]
                next_node = ndList[i]
                action = maze.getAction(car_dir, current_node, next_node)
                # current car position + to node => get action
                # print("Get Action: ", action, "\n")
                print("Current Car direction: ", car_dir)
                if action == mz.Action.ADVANCE:
                    car_dir = car_dir
                elif action == mz.Action.U_TURN:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.NORTH
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.WEST
                elif action == mz.Action.TURN_RIGHT:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.WEST
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.NORTH
                elif action == mz.Action.TURN_LEFT:
                    if car_dir == Direction.NORTH:
                        car_dir = Direction.WEST
                    elif car_dir == Direction.WEST:
                        car_dir = Direction.SOUTH
                    elif car_dir == Direction.SOUTH:
                        car_dir = Direction.EAST
                    elif car_dir == Direction.EAST:
                        car_dir = Direction.NORTH
                # When car arrive to a node !!!
                while(1):
                    python_get_information = interf.ser.SerialReadString()
                    print(python_get_information)
                    if python_get_information is 'N':
                        count = count + 1
                        print(python_get_information)
                        print("Get to a node!!\n")
                        break
                # python_get_information = interf.ser.SerialReadString()
                # while python_get_information is not 'N':
                #     python_get_information = interf.ser.SerialReadString()
                # print("Get to a node!!")
                # while(1):
                #     python_get_information = interfinterface.ser.SerialReadString()
                #     if python_get_information is 'N':
                #         print(python_get_information)
                #         print("Get to a node!!\n")
                #         break
                # Send the state to Arduino
                print("Get action: ", action)
                interf.send_action(action)
                # node = 0
                # get_UID = "just a test"
                # point.add_UID(get_UID)
            print("Get action: ", mz.Action.HALT)
            interf.send_action(mz.Action.HALT)
            break

    # Mode 2: for directly remote-controlling the car via HC-05.
    elif sys.argv[1] == '2':
        print("Mode 2: for directly controlling the car via HC-05.")
        while True:
            state_cmd = input("Please enter a remote-controlled command: ")
            interf.ser.SerialWrite(state_cmd)


if __name__=='__main__':
    main()

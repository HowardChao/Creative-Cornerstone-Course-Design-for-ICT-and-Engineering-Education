from node import *
import maze as mz
import score
import student
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
    interface = student.interface()         # the part of calling student.py was commented out.

    # Do not have destination
    if (sys.argv[1] == '0'):
        print("Mode 0")
        while (1):

            #TODO: Impliment your algorithm here and return the UID for evaluation function
            # ================================================
            # Basically, you will get a list of nodes and corresponding UID strings after the end of algorithm.
			# The function add_UID() would convert the UID string score and add it to the total score.
			# In the sample code, we call this function after getting the returned list. 
            # You may place it to other places, just make sure that all the UID strings you get would be converted.
            # ================================================
            # for i in ndList:
            #     print(i.getIndex())
            # ndList2 = maze.strategy(ndList[-1])
            # for i in ndList2:
            #     print(i.getIndex())
            # ndList3 = maze.strategy(ndList2[-1])
            # for i in ndList3:
            #     print(i.getIndex())
            ndList = []
            deadend_node_num = 0
            for node in maze.nodes:
                if len(node.getSuccessors()) == 1:
                    deadend_node_num += 1
            start_nd = next_nd
            BFS_list = []
            for i in range(1, deadend_node_num+1):
                BFS_list_run = maze.strategy(start_nd)
                BFS_list = BFS_list_run
                start_nd = BFS_list[-1]
                if i == deadend_node_num:
                    ndList = ndList + BFS_list
                else:
                    ndList = ndList + BFS_list[:-1]
                for j in BFS_list:
                    print(j.getIndex())
            
            ## Check the result for the whole BFS!!!
            for node in ndList:
                print(node.getIndex())
            state_cmd = input("Please enter a mode command: ")
            interface.ser.SerialWrite(state_cmd)
            state_cmd = input("Please enter a mode command: ")
            interface.ser.SerialWrite(state_cmd)
        ## Testing encounter the node !!
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
                    python_get_information = interface.ser.SerialReadString()
                    if python_get_information is 'N':
                        count = count + 1
                        print("Get to a node!!\n")
                        break
                # python_get_information = interface.ser.SerialReadString()
                # while python_get_information is not 'N':
                #     python_get_information = interface.ser.SerialReadString()
                # print("Get to a node!!")
                # while(1):
                #     python_get_information = interface.ser.SerialReadString()
                #     if python_get_information is 'N':
                #         print(python_get_information)
                #         print("Get to a node!!\n")
                #         break
                # Send the state to Arduino
                print("Get action: ", action)
                interface.send_action(action)
                # node = 0
                # get_UID = "just a test"
                # point.add_UID(get_UID)
            print("Get action: ", mz.Action.HALT)
            interface.send_action(mz.Action.HALT)
            break

    # Have destination
    elif (sys.argv[1] == '1'):
        print("Mode 1")
        while (1):
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
        ###Testing for getting into a node !!
            state_cmd = input("Please enter a mode command: ")
            interface.ser.SerialWrite(state_cmd)
            state_cmd = input("Please enter a mode command: ")
            interface.ser.SerialWrite(state_cmd)
        ## Testing encounter the node !!
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
                    python_get_information = interface.ser.SerialReadString()
                    print(python_get_information)
                    if python_get_information is 'N':
                        count = count + 1
                        print(python_get_information)
                        print("Get to a node!!\n")
                        break
                # python_get_information = interface.ser.SerialReadString()
                # while python_get_information is not 'N':
                #     python_get_information = interface.ser.SerialReadString()
                # print("Get to a node!!")
                # while(1):
                #     python_get_information = interface.ser.SerialReadString()
                #     if python_get_information is 'N':
                #         print(python_get_information)
                #         print("Get to a node!!\n")
                #         break
                # Send the state to Arduino
                print("Get action: ", action)
                interface.send_action(action)
                # node = 0
                # get_UID = "just a test"
                # point.add_UID(get_UID)
            print("Get action: ", mz.Action.HALT)
            interface.send_action(mz.Action.HALT)
            break

    # Our testing mode
    elif (sys.argv[1] == '2'):
        print("Mode 2")
        while(1):
            state_cmd = input("Please enter a mode command: ")
            # if state_cmd == 's':
            #     print(state_cmd)
            # if state_cmd == '':
            #     print(state_cmd)
            # if state_cmd == 's':
            #     print(state_cmd)
            # if state_cmd == 's':
            #     print(state_cmd)
            # if state_cmd == 's':
            #     print(state_cmd)
            # if state_cmd == 's':
            #     print(state_cmd)

            interface.ser.SerialWrite(state_cmd)
            # interface.ser.SerialReadString()
            # if state_cmd == "break":
                
            #     break
    """
    node = 0
    while(not node):
        node = interface.wait_for_node()

    interface.end_process()
    """
    print("complete")
    print("")
    a = point.getCurrentScore()
    print("The total score: ", a)

if __name__=='__main__':
    main()
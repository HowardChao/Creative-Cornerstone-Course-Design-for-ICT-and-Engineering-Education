from node import *
import maze as mz
import score
import sys
import requests

def main():
    # Initialization
    # 1. Read map csv file
    maze = mz.Maze("data/maze_complex.csv")
    # 2. Valid UID file
    point = score.Scoreboard("data/UID.csv")
    # 3. Team name
    team_name = "Team_XXX"
    # 4. Judge server IP
    ip = "114.34.123.174:5000"
    # 5. Car direction "South"
    ip = "114.34.123.174:5000"
    # 6. Other variable

    # Mode 0: for treasure-hunting with rule 1
    if (sys.argv[1] == '1'):  
        print("Mode 1: Game Start!")
        # TODO: Finish Game rule 1

        # When you start the game, call the following judge code.
        ######### Do not change #########
        ########## Juge code ############
        url_start = 'http://' + ip + '/{}/start/'.format(
            team_name)
        r = requests.get(url_start)
        #################################


        # When you get an UID, please call the following judge code. 
        ######### Do not change #########
        ########## Juge code ############
        current_score = point.getCurrentScore()
        url = url = 'http://' + ip + '/{}/{}/'.format(
            team_name, current_score)
        r = requests.get(url_start)
        #################################



    # Mode 1: for treasure-hunting with rule 2
    elif (sys.argv[1] == '2'):
        print("Mode 2: Game Start!")
        # TODO: Finish Game rule 2

        # When you start the game, call the following judge code.
        ######### Do not change #########
        ########## Juge code ############
        url_start = 'http://' + ip + '/{}/start/'.format(
            team_name)
        r = requests.get(url_start)
        #################################


        # When you get an UID, please call the following judge code. 
        ######### Do not change #########
        ########## Juge code ############
        current_score = point.getCurrentScore()
        url = url = 'http://' + ip + '/{}/{}/'.format(
            team_name, current_score)
        r = requests.get(url_start)
        #################################



    # Mode 2: Self-testing mode.
    elif (sys.argv[1] == '3'):
        print("Mode 3: Self-testing mode.")
        # TODO: You can write your code to test specific function.


if __name__ == '__main__':
    main()

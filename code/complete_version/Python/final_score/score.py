from flask import Flask, url_for, jsonify, request, render_template
import json
import datetime
app = Flask(__name__)

# April 5, 2019 15:37:25

# team= {}
# # There are total 24 teams!
# for i in range(24):
#     team[i] = {"score": 0, "time": 0}
# print(team)

team = "0"
global_score = 0
start_bool = False
end_bool = False
game_over_time = datetime.datetime.now()



@app.route('/', methods=['GET'])
def hello_world():
    return render_template('index.html')

@app.route('/<username>/start/')
def team_start(username):
    global start_bool
    global game_over_time
    global current_time
    if start_bool is False:
        start_bool = True
        now_time = datetime.datetime.now()
        game_over_time = now_time + datetime.timedelta(0,180)
        print("now_time: ", now_time)
        print("game_over_time: ", game_over_time)
        time_difference = (game_over_time - now_time).total_seconds()
        time_difference = int(round(time_difference))
        print("time_difference: ", time_difference)
    elif start_bool is True:
        pass
    return None
    # new_strip_time = datetime.datetime.now().strftime("%B %d, %Y %H:%M+60:%S ")
    # return render_template('race_counting.html', team=username, start_bool = start_bool, end_bool = end_bool, remain_time = time_difference)

@app.route('/<username>/end/')
def team_end(username):
    global end_bool
    end_bool = True
    # return render_templsate('race_counting.html', team=username, start_bool = start_bool, end_bool = end_bool)

@app.route('/<username>/reset/')
def time_reset(username):
    global start_bool
    global end_bool
    global game_over_time
    global global_score
    start_bool = False
    end_bool = False
    global_score = 0
    game_over_time = datetime.datetime.now()
    # return render_template('race_counting.html', team=username, start_bool = start_bool, end_bool = end_bool)

# <username> type is path
@app.route('/<username>/')
def team_home_page(username):
    # show the user profile for that user
    return render_template('team.html', team=username)

# <username> type is path
@app.route('/<username>/<score>/')
def start_racing(username, score):
    global game_over_time
    global global_score
    global_score = score
    current_time = datetime.datetime.now()
    time_difference = (game_over_time - current_time).total_seconds()
    time_difference = int(round(time_difference, 0))
    print("time_difference: ", time_difference)
    if time_difference <= 0:
        time_difference = 0
    # return render_template('race_counting.html', team=username, score=score, start_bool = start_bool, end_bool = end_bool, remain_time = time_difference)
@app.route('/<username>/score/')
def start_racing_gobal_score(username):
    global game_over_time
    global global_score
    global start_bool
    if start_bool is False:
        pass
    elif start_bool is True:
        pass
    current_time = datetime.datetime.now()
    time_difference = (game_over_time - current_time).total_seconds()
    time_difference = int(round(time_difference, 0))
    print("time_difference: ", time_difference)
    if time_difference <= 0:
        time_difference = 0
    return render_template('race_counting.html', team=username, score=global_score, start_bool = start_bool, end_bool = end_bool, remain_time = time_difference)

if __name__ == '__main__':
    question_name = []
    app.run( debug = True, host='0.0.0.0')

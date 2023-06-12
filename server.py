from flask import Flask, render_template
from flask import request
import csv
import datetime
app = Flask(__name__)

valueBright = 0
state = 0

@app.route("/", methods=['GET', 'POST'])

def hello():
    # Use global variable
    global valueBright
    global state
    current_time = datetime.datetime.now()
    timestamp = current_time.strftime("%Y-%m-%d %H:%M:%S")
    # Update global value when req returns not none
    if request.args.get("var") != None:
        valueBright = request.args.get("var")
    if request.args.get("state") != None:
        state = request.args.get("state")
    print("Brightness value:", valueBright)
    filename = "data.csv"
    with open (filename, "a", newline = "") as file:
        writer = csv.writer(file)
        writer.writerow([current_time, valueBright])
    if (state == "0" ):
        state_string = " Touch Sensor "
    elif(state == "1"):
        state_string = " Auto - Brightness "
    return render_template("index.html", brightness = valueBright, switch = state_string)


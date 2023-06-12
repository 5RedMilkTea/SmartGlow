from flask import Flask, render_template
from flask import request
import csv
import datetime
app = Flask(__name__)

valueBright = -1


@app.route("/", methods=['GET', 'POST'])

def hello():
    # Use global variablei
    global valueBright
    current_time = datetime.datetime.now()
    timestamp = current_time.strftime("%Y-%m-%d %H:%M:%S")
    # Update global value when req returns not none
    if request.args.get("var") != None:
        valueBright = request.args.get("var")
    print("Brightness value:", valueBright)
    filename = "data.csv"
    with open (filename, "a", newline = "") as file:
        writer = csv.writer(file)
        writer.writerow([current_time, valueBright])
    return render_template("index.html", brightness = valueBright)


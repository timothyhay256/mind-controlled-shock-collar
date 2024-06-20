headsetHost = "192.168.86.77"  
headsetPort = 4120
collarHost = "192.168.12.150" # Note that headset packets require a response from server, but collar packets are just sent out 
# RPI is 192.168.12.230
collarPort = 4120 
outFile = "data"
optionFile = "options" # All variables between flask and main must be done via files. Also allows Unity to get data and options.

from headset import main as headsetLoop
from animate_graph import graph
from animate_bar import bar
from simpleLog import log
from utils.gen_data import genData
import socket
import threading
import time
import subprocess
from multiprocessing import Process
import multiprocessing
from flask import Flask, render_template, jsonify, request, redirect
import csv

punishAttention = 1 # Punish attention if 0: Don't punish, 1: If it goes above the threshold, 2: If it goes below the threshold
punishMeditation = 0
threshold = 210 # By default set to an impossible value so nobody gets hurt during setup 
channel = "1" # Collar channel
mode = "4" # 1: LED, 2: BEEP, 3: VIBRATE, 4: ZAP
timeout = 5 # Punishment timeout in seconds
severityDivider = 2 # Divide amount away from threshold by this value

animWith = 2 # 0: graph, 1: bar, 2: web (also enables controls)
eStop = False # When set to true, no outbound packets are sent
fakeData = False # If I should generate fake data. Used for debugging.
debug = True # Log bugLog?

written = threading.Event() # Allows blocking until new data is written
exitAll = threading.Event() # Exit.
initialWritten = multiprocessing.Event()

log("WARNING: SHOCK IS CURRENTLY OVERRIDED AT LEVEL 100!!!", "e") # remove later

def sendCollar(host, port, command):
    global eStop
    if eStop:
        log("EStop is enabled! I cannot send packets until you restart the program", "e")
    else:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(command.encode(), (host, port))

def read_csv(filename):
    data = []
    with open(filename, 'r') as file:
        csv_reader = csv.DictReader(file)
        for row in csv_reader:
           data.append(row)
    return data

def rwOptions(write=False, threshold=0, punishAttention=0, punishMeditation=0, punishWith=3, wantOption=0): # Option 0 is threshold, 1 is punishAttention, 2 is punishMeditation, 3 is punishWith
    if write == False:
        # bugLog("Options are being read from!")
        options = open(optionFile, 'r')
        if wantOption == 0:
            return(options.read().split(",")[0])
        elif wantOption == 1:
            return(options.read().split(",")[1])
        elif wantOption == 2:
            return(options.read().split(",")[2])
        elif wantOption == 3:
            return(options.read().split(",")[3])
    elif write:
        bugLog("Options are being written to!")
        options = open(optionFile, 'w')
        options.write(str(threshold)+","+str(punishAttention)+","+str(punishMeditation)+","+str(punishWith))
        options.flush()
        options.close()

def bugLog(message, level="i"):
    global debug
    if debug:
        log(message, level, "DEBUG")


headsetThread = threading.Thread(target=headsetLoop, args=(headsetHost, headsetPort, outFile, written, exitAll), daemon=True)
headsetThread.start()

rwOptions(True, threshold, punishAttention, punishMeditation, mode)

log("\n\nHello Open Sauce!!! Begin!\n\n")

if fakeData:
    log("Starting thread to genearate fake data!", "w")
    threading.Thread(target=genData, args=(outFile,), daemon=True).start()
if animWith == 0:
    #graphThread = threading.Thread(target=graph, args=(outFile, written, exitAll), daemon=True).start()
    graphProc = Process(target=graph, args=(outFile, initialWritten))
    graphProc.start()
elif animWith == 1:
    graphProc = Process(target=bar, args=(outFile, initialWritten))
    graphProc.start()
elif animWith == 2:
    log("Starting web interface")
    app = Flask(__name__)
    @app.route('/')
    def index():
        return render_template('index.html')

    @app.route('/graph')
    def graph():
        return render_template('graph.html')

    @app.route('/play')
    def play():
        threshold = rwOptions(wantOption=0)
        if punishMeditation != 0:
            # log("Setting meditation threshold to "+str(threshold))
            data = {'threshold': threshold}
        elif punishAttention != 0:
            # log("Setting meditation threshold to "+str(threshold))
            data = {'threshold': threshold}
        print(data)
        return render_template('play.html', data=data)

    @app.route('/open-sussy') # Seperate route because I plan to add more mode specific stuff later (LED stuff)
    def open_sussy():
        threshold = rwOptions(wantOption=0)
        if punishMeditation != 0:
            # log("Setting meditation threshold to "+str(threshold))
            data = {'threshold': threshold}
        elif punishAttention != 0:
            # log("Setting meditation threshold to "+str(threshold))
            data = {'threshold': threshold}
        print(data)
        return render_template('openSauce.html', data=data)

    @app.route('/shock')
    def shock():
        level = request.args.get('power')
        log("Got request to send a shock of power level " + level + "!")
        sendCollar(collarHost, collarPort, channel+mode+"100")
        # return 'Successfully sent shock of power level '+str(level)
        return redirect('/play')

    # @app.route('/restart-headset')
    # def restart():
    #     log("Got request to restart headset unishWith
    #         pass
    #     threading.Thread(target=headsetLoop, args=(headsetHost, headsetPort, outFile, written, exitAll),
    #                      daemon=True).start()
    #     exitAll.clear()
    #     return 'Successfully killed old headset thread and started new one'

    # Route to fetch data from CSV file
    @app.route('/headset-data')
    def get_data():
        return jsonify(read_csv(outFile))

    @app.route('/stop')
    def stop():
        log("E-Stop hit, halting all outbound packets!", "e")
        eStop = True
        rwOptions(True, 500, 0, 0, 3) # Sets threshold to unreachable value, and sets activation mode to buzz
        return render_template('stop.html')

    @app.route("/options" , methods=['GET', 'POST'])
    def options():
        typePunish = request.args.get('type')
        condition = request.args.get('condition')
        threshold = request.args.get('threshold')
        mode = request.args.get('punishment')
        if typePunish == "meditation":
            if condition == "above":
                punishMeditation = 1
            elif condition == "below":
                punishMeditation = 2
            punishAttention = 0
            rwOptions(True, threshold, punishAttention, punishMeditation, mode)
            bugLog("Setting meditation threshold to "+threshold)
        elif typePunish == "attention":
            if condition == "above":
                punishAttention = 1
            elif condition == "below":
                punishAttention = 2
            punishMeditation = 0
            rwOptions(True, threshold, punishAttention, punishMeditation, mode)
            bugLog("Setting attention threshold to "+threshold)
        else:
            log("Invalid data was submitted to the form!", "e")
        bugLog("New options were set. punishMeditation: "+str(punishMeditation)+" punishAttention:"+str(punishAttention)+" threshold:"+str(threshold)+" mode:"+str(mode))
        # return redirect('/play')
        return redirect('open-sussy') # Temp for Open Sauce


    webThread = threading.Thread(target=lambda: app.run(debug=True,use_reloader=False), daemon=True).start()

else:
    log("No visualization method has been selected!", "w")

def main():
    global written
    severity = 0
    written.wait() # Wait for new data
    initialWritten.set() # Unblock animator
    written.clear()
    line = str(subprocess.check_output(['tail', '-1', '/home/jamesh/Brain/'+outFile]).decode('utf-8')).strip("\n").split(",") # TODO: Make this not stupid
    threshold = int(rwOptions(wantOption=0)) # TODO: Make this not stupid and garbage
    punishAttention = int(rwOptions(wantOption=1))
    punishMeditation = int(rwOptions(wantOption=2))
    mode = int(rwOptions(wantOption=3))
    bugLog("Current settings are punishMeditation: "+str(punishMeditation)+" punishAttention:"+str(punishAttention)+" threshold:"+str(threshold)+" mode:"+str(mode))
    bugLog("Got threshold - currently set to "+str(threshold))
    if int(line[1]) == 0:
        if punishAttention == 1 and int(line[2]) > threshold or punishAttention == 2 and int(line[2]) < threshold:
            if punishAttention == 1:
                log("Attention "+line[2]+" is above threshold "+str(threshold)+"!")
                severity = int(line[2]) - threshold
            if punishAttention == 2:
                log("Attention "+line[2]+" is below threshold "+str(threshold)+"!")
                severity = threshold - int(line[2])
            log("Punishing attention with severity of "+str(severity)+"! Timing out for "+str(timeout)+" seconds.")
            if animWith == 1 or 2:
                subprocess.run(["touch",  "mark"]) # Mark spot where threshold was hit. animate_graph will remove the file.
            level = str(int(round((severity / severityDivider), 0)))
            if int(level) == 0:
                log("Severity results in level " + str(1) + " shock.")
                level = "1"
            else:
                log("Severity results in level " + level + " shock.", "e")
            sendCollar(collarHost, collarPort, str(channel) + str(mode) + "100") # 6 tier
            # print(channel+mode+level)
            time.sleep(timeout)
        elif punishMeditation == 1 and int(line[3]) > threshold or punishMeditation == 2 and int(line[3]) < threshold:
            if punishMeditation == 1:
                log("Meditation "+line[3]+" is above threshold "+str(threshold)+"!")
                severity = int(line[3]) - threshold
            if punishMeditation == 2:
                log("Meditation "+line[3]+" is below threshold "+str(threshold)+"!")
                severity = threshold - int(line[3])
            log("Punishing meditation with severity of "+str(severity)+"! Timing out for "+str(timeout)+" seconds.")
            if animWith == 1 or 2:
                subprocess.run(["touch",  "mark"]) # Mark spot where threshold was hit. animate_graph will remove the file.
            level = str(int(round((severity / severityDivider), 0)))  # str(int(round((severity / (maxLevelOffset/6)), 0)))
            if int(level) == 0:
                log("Severity results in level " + str(1) + " shock.")
                level = "1"
            else:
                log("Severity results in level " + level + " shock.", "e")
            # print(channel+mode+level)
            sendCollar(collarHost, collarPort, str(channel) + str(mode) + "100") # 6 tier
            time.sleep(timeout)
        else:
            log("You are within your safe limits.")
    else:
        log("Strength is not ideal. I will not shock until strength reaches 0!", "w")

log("The program will wait for contact with the headset.")
try:
    while True:
        main()
except (KeyboardInterrupt, SystemExit):
    log("Received keyboard interrupt, exiting all threads.", "e")
    exitAll.set()
    exit(0)

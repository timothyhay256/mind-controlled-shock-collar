collarHost = "192.168.1.47" # Note that headset packets require a response from server, but collar packets are just sent out 
collarPort = 4120 
from time import sleep
import socket
from simpleLog import log
import subprocess

# logPath = '/home/jamesh/.local/share/PolyMC/instances/Other 1.20.4/.minecraft/logs/latest.log'
logPath = '/home/jamesh/.local/share/PolyMC/instances/1.20.4/.minecraft/logs/latest.log'
lastLine = ""
currentHealthP1 = 0 # TODO: A better solution than this
previousHealthP1 = 20 # Assume full health on load
currentHealthP2 = 0
previousHealthP2 = 20

lowHealth = False # Custom rules for maps that change the health the player has. Detects when the health goes from 20 to another arbitrary value.
lowHealthLevel = "20" # Level of punishment delivered when player death is detected in lowHealth mode

lowHealthTrigger = 14 # Triggers punishment when this much damage is taken, so if set to 14, 20 to 6 will trigger.

p1 = "timothyhay"
p2 = "snoopyhoff"

players = 2 # How many players to use. p1 uses channel 1 by default.

channel = "1"
mode = "4" # 1: LED, 2: BEEP, 3: VIBRATE, 4: ZAP

shockMultiplier = 5

block = False # If set to true, no commands will be sent. Used for lowHealth
if mode == "4":
    log("WARNING -- You are running with shocks ENABLED! -- WARNING", "w")
    log("WARNING -- With a multiplier of "+str(shockMultiplier)+", the max shock level is "+str(shockMultiplier*20)+"! -- WARNING", "w")

def getLastLine(filePath):
    with open(filePath, 'r') as file:
        lines = file.readlines()
        lastLine = lines[-1].strip()  # remove newline character
    return lastLine

def sendCollar(host, port, command):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(command.encode(), (host, port))

def pairCollar(): # Call me before the loop to pair collars
    while True:
        chan = input("CHANNEL (q to quit): ")
        if chan == "q":
            exit()
        elif chan == "1":
            log("Sending channel 1")
            sendCollar(collarHost, collarPort, "1350")
        elif chan == "2":
            log("Sending channel 2")
            sendCollar(collarHost, collarPort, "2350")
        else:
            log("Sending input as raw!")
            sendCollar(collarHost, collarPort, str(chan))

# pairCollar()

log("I will monitor the Minecraft logfile located at "+logPath+" and fire packets at "+collarHost+":"+str(collarPort))
while True:
    # sleep(.1)
    # currentLine = getLastLine(logPath)
    currentLine = str((subprocess.check_output(['tail', '-1', logPath])).decode('utf-8')[0:-1])
    # if lastLine != currentLine:
    if "~" in currentLine:
        line = currentLine.split("~")
        # log(line[1]) # Player name
        # log(line[3]) # Player health
        if line[1] == p1:
            # log("fsd")
            # log(currentHealthP1)
            # log(previousHealthP1)
            currentHealthP1 = line[3]
            if currentHealthP1 != previousHealthP1:
                damage =  (int(currentHealthP1) - int(previousHealthP1))*-1
                log(line[1]+" health: "+currentHealthP1)
                if damage > 0:
                    log(line[1]+" damage: "+str(damage))
                    level = str(int(damage*shockMultiplier))
                    if 0 < int(level) <= 100:
                        if damage == lowHealthTrigger and lowHealth:
                            log(line[1]+" gets a level "+lowHealthLevel+" shock on channel 1.", "w")
                            sendCollar(collarHost, collarPort, "1"+mode+lowHealthLevel)
                            block = True
                        else:
                            block = False
                        if not block and not lowHealth:
                            log(line[1]+" gets a level "+level+" shock on channel 1.", "w")
                            sendCollar(collarHost, collarPort, "1"+mode+level)
        elif line[1] == p2:
            currentHealthP2 = line[3]
            if currentHealthP2 != previousHealthP2:
                damage =  (int(currentHealthP2) - int(previousHealthP2))*-1
                log(line[1]+" health: "+currentHealthP2)
                if damage > 0:
                    log(line[1]+" damage: "+str(damage))
                    level = str(int(damage*shockMultiplier))
                    if 0 < int(level) <= 100:
                        if damage == lowHealthTrigger and lowHealth:
                            print("fds")
                            log(line[1]+" gets a level "+lowHealthLevel+" shock on channel 2.", "w")
                            sendCollar(collarHost, collarPort, "2"+mode+lowHealthLevel)
                            block = True
                        else:
                            block = False
                        if not block and not lowHealth:
                            print("asfed")
                            log(line[1]+" gets a level "+level+" shock on channel 2.", "w")
                            sendCollar(collarHost, collarPort, "2"+mode+level)
        # currentHealth = int(currentLine.split("~")[1])
        # damage = (currentHealth - previousHealth)*-1 # Get actual damage
        # if currentHealth != previousHealth:
        #     log("Health: "+str(currentHealth))
        #     if damage > 0:
        #         log("Damage: "+str(damage)+" damage.")
        #         level = str(int(damage*shockMultiplier)) 
        #         if 0 < int(level) <= 100: 
        #             log("Player gets a level "+level+" shock.")
        #             sendCollar(collarHost, collarPort, channel+mode+level)
        
    else:
        # log(currentLine)
        pass
    lastLine = currentLine
    previousHealthP1 = currentHealthP1
    previousHealthP2 = currentHealthP2

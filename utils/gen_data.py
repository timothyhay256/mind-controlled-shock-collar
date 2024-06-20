from time import sleep
import random
import os

def genData(filename):
    os.system('rm '+filename)
    i = 0

    file = open(filename, "w")
    file.write("index,strength,attention,meditation,delta,theta,low_alpha,high_alpha,low_beta,high_beta,low_gamma,high_gamma")
    file.write("\n"+str(i)+","+str(random.randint(0, 200))+","+str(random.randint(0, 200))+","+str(random.randint(0, 200))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000)))
    file.flush()

    while True:
        sleep(.5)
        file.write("\n"+str(i)+","+str(random.randint(0, 100))+","+str(random.randint(0, 100))+","+str(random.randint(0, 100))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000))+","+str(random.randint(0, 100000)))
        i += 1
        file.flush()

import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib import style
from os.path import exists
import os
import time
limit_points = 0 # TODO: Fix markers not following when points are limited.
pop = 0
marked = []
index = 0

def graph(file, event):
    event.wait()
    style.use('seaborn-v0_8')

    fig, ax = plt.subplots(3, 4, figsize=(19, 11))
    ax = ax.flatten()
    figManager = plt.get_current_fig_manager()
    figManager.window.showMaximized()
    plt.tight_layout()
    def animate(i):
        global limit_points, pop, marked, index
        graph_data = open(file,'r').read()
        lines = graph_data.split('\n')
        xs = []
        y_strength = []
        y_attention = []
        y_meditation = []
        y_delta = []
        y_theta = []
        y_low_alpha = []
        y_high_alpha = []
        y_low_beta = []
        y_high_beta = []
        y_low_gamma = []
        y_high_gamma = []
        for line in lines:
            if len(line) > 1:
                index, strength, attention, meditation, delta, theta, low_alpha, high_alpha, low_beta, high_beta, low_gamma, high_gamma = line.split(",")
                y_strength.append(float(strength))
                y_attention.append(float(attention))
                y_meditation.append(float(meditation))
                y_delta.append(float(delta))
                y_theta.append(float(theta))
                y_low_alpha.append(float(low_alpha))
                y_high_alpha.append(float(high_alpha))
                y_low_beta.append(float(low_beta))
                y_high_beta.append(float(high_beta))
                y_low_gamma.append(float(low_gamma))
                y_high_gamma.append(float(high_gamma))
                xs.append(float(index))
        #for i in range(1,12):
            #eval("ax"+str(i)+".clear()")
        #ax.clear()
        if limit_points > 0:
            if int(index) > limit_points:
                pop = 0
                for i in range(int(index)-limit_points):
                    if len(xs) != 0:
                        xs.pop(0)
                        y_strength.pop(0)
                        y_attention.pop(0)
                        y_meditation.pop(0)
                        y_delta.pop(0)
                        y_theta.pop(0)
                        y_low_alpha.pop(0)
                        y_high_alpha.pop(0)
                        y_low_beta.pop(0)
                        y_high_beta.pop(0)
                        y_low_gamma.pop(0)
                        y_high_gamma.pop(0)
                        pop += 1
                if len(marked) != 0:
                    for i in range(len(marked)):
                        marked[i] = marked[i] - 1
                    if marked[0] == 0:
                        marked.pop(0)

        file_exists = exists("./mark")
        if file_exists:
            print("Marking " + str(xs[len(xs) - 1])) #Mark in CSV file?
            marked.append(int(xs[len(xs) - 1]-pop))
            print("Marked: " + str(marked) + " Pop: " + str(pop))
            pop = 0
            os.remove("./mark")
        for i in range(12):
            ax[i].clear()
        if len(y_strength) == 0: #If the CSV restarts
            print("Clearing marked")
            marked = []
        ax[0].plot(xs, y_strength, color="b", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_attention, color="g", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_meditation, color="r", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_delta, color="c", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_theta, color="m", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_low_alpha, color="y", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_high_alpha, color="k", markevery=marked, marker="D", markerfacecolor="white")
        ax[0].plot(xs, y_low_beta, color="b", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_high_beta, color="g", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_low_gamma, color="r", markevery=marked, marker="D", markerfacecolor="black")
        ax[0].plot(xs, y_high_gamma, color="c", markevery=marked, marker="D", markerfacecolor="black")

        if len(y_strength) > 0:
            if int(y_strength[len(y_strength)-1]) != 0:
                ax[1].plot(xs, y_strength, label="strength", color="r", markevery=marked, marker="D", markerfacecolor="black")
            else:
                ax[1].plot(xs, y_strength, label="strength", color="b", markevery=marked, marker="D", markerfacecolor="black")
        ax[2].plot(xs, y_attention, label="attention", color="g", markevery=marked, marker="D", markerfacecolor="black")
        ax[3].plot(xs, y_meditation, label="meditation", color="r", markevery=marked, marker="D", markerfacecolor="black")
        ax[4].plot(xs, y_delta, label="delta", color="c", markevery=marked, marker="D", markerfacecolor="black")
        ax[5].plot(xs, y_theta, label="theta", color="m", markevery=marked, marker="D", markerfacecolor="black")
        ax[6].plot(xs, y_low_alpha, label="low_alpha", color="y", markevery=marked, marker="D", markerfacecolor="black")
        ax[7].plot(xs, y_high_alpha, label="high_alpha", color="k", markevery=marked, marker="D", markerfacecolor="white")
        ax[8].plot(xs, y_low_beta, label="low_beta", color="b", markevery=marked, marker="D", markerfacecolor="black")
        ax[9].plot(xs, y_high_beta, label="high_beta", color="g", markevery=marked, marker="D", markerfacecolor="black")
        ax[10].plot(xs, y_low_gamma, label="low_gamma", color="r", markevery=marked, marker="D", markerfacecolor="black")
        ax[11].plot(xs, y_high_gamma, label="high_gamma", color="c", markevery=marked, marker="D", markerfacecolor="black")
        for i in range(12):
            ax[i].legend()

    print("animate_graph: Starting animation")
    ani = animation.FuncAnimation(fig, animate, interval=500)
    plt.show()
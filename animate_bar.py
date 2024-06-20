import matplotlib.pyplot as plt #Alot dirtier than animate_graph
import matplotlib.animation as animation
from matplotlib import style

delta_lim = 3500000
theta_lim = 1320000
low_alpha_lim = 600700
high_alpha_lim = 200000
low_beta_lim = 200000
high_beta_lim = 152500
low_gamma_lim = 163000
high_gamma_lim = 125000
def bar(file, event):
    event.wait()
    style.use('seaborn-v0_8')

    fig = plt.figure(figsize=(19, 11))
    for i in range(1, 12):
        exec("ax"+str(i)+" = fig.add_subplot(3,4,"+str(i)+")")
    figManager = plt.get_current_fig_manager()
    figManager.window.showMaximized()
    plt.tight_layout()
    def animate(i):
        global delta_lim, theta_lim, low_alpha_lim, high_alpha_lim, low_beta_lim, high_beta_lim, low_gamma_lim, high_gamma_lim, ax1, ax2, ax3, ax4, ax5, ax6, ax7, ax8, ax9, ax10, ax11
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
                y_strength=(float(strength))
                y_attention=(float(attention))
                y_meditation=(float(meditation))
                y_delta=(float(delta))
                y_theta=(float(theta))
                y_low_alpha=(float(low_alpha))
                y_high_alpha=(float(high_alpha))
                y_low_beta=(float(low_beta))
                y_high_beta=(float(high_beta))
                y_low_gamma=(float(low_gamma))
                y_high_gamma=(float(high_gamma))
                xs=(float(index))
        for i in range(1,12):
            eval("ax"+str(i)+".clear()")
        ax1.set_ylim(0, 200)
        ax2.set_ylim(0, 100)
        ax3.set_ylim(0, 100)
        if y_delta > delta_lim:
            delta_lim = y_delta
        if y_theta > theta_lim:
            theta_lim = y_theta
        if y_low_alpha > low_alpha_lim:
            low_alpha_lim = y_low_alpha
        if y_high_alpha > high_alpha_lim:
            high_alpha_lim = y_high_alpha
        if y_low_beta > low_beta_lim:
            low_beta_lim = y_low_beta
        if y_high_beta > low_beta_lim:
            low_beta_lim = y_high_beta
        if y_low_gamma > low_gamma_lim:
            low_gamma_lim = y_low_gamma
        if y_high_gamma > high_gamma_lim:
            high_gamma_lim = y_high_gamma
        ax4.set_ylim(0, delta_lim)
        ax5.set_ylim(0, theta_lim)
        ax6.set_ylim(0, low_alpha_lim)
        ax7.set_ylim(0, high_alpha_lim)
        ax8.set_ylim(0, low_beta_lim)
        ax9.set_ylim(0, high_beta_lim)
        ax10.set_ylim(0, high_gamma_lim)
        ax11.set_ylim(0, low_gamma_lim)
        ax1.bar(1, y_strength, label="strength", color="b")
        ax2.bar(1, y_attention, label="attention", color="g")
        ax3.bar(1, y_meditation, label="meditation", color="r")
        ax4.bar(1, y_delta, label="delta", color="c")
        ax5.bar(1, y_theta, label="theta", color="m")
        ax6.bar(1, y_low_alpha, label="low_alpha", color="y")
        ax7.bar(1, y_high_alpha, label="high_alpha", color="k")
        ax8.bar(1, y_low_beta, label="low_beta", color="b")
        ax9.bar(1, y_high_beta, label="high_beta", color="g")
        ax10.bar(1, y_low_gamma, label="low_gamma", color="r")
        ax11.bar(1, y_high_gamma, label="high_gamma", color="c")
        for i in range(1, 12):
            eval("ax" + str(i) + ".legend()")

    print("animate_bar: Starting animation")
    ani = animation.FuncAnimation(fig, animate, interval=800)
    plt.show()

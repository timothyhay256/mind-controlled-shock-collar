import socket
import select
from simpleLog import log

def main(host, port, outFile, event, exitAll):
	log("Headset: Going to talk to "+str(host)+":"+str(port))
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.setblocking(False)
	sock.sendto("START".encode(), (host, port))
	f = open(outFile, "w")
	f.close()
	f = open(outFile, "a")
	i = 0

	while True:
		if exitAll.is_set():
			log("headset exiting prematurely.")
			exit(0)
			break
		ready = select.select([sock], [], [], 3)
		if (ready[0]):
			# log("Data is ready!")
			i += 1
			data, addr = sock.recvfrom(512)
			data = data.decode("utf-8")
			sock.sendto("ACK".encode(), (host, port))
			if i == 1:
				log("Headset: Writing header and 0's to set baseline")
				f.write("index,strength,attention,meditation,delta,theta,low_alpha,high_alpha,low_beta,high_beta,low_gamma,high_gamma\n")
				f.write("0,0,0,0,0,0,0,0,0,0,0,0\n")
			f.write(str(i)+","+data+"\n")
			f.flush()
			log("Headset: Writing packet and unblocking threads")
			event.set() # Unblocks all other threads
		elif i != 0:
			if exitAll.is_set():
				log("headset exiting prematurely.")
				exit(0)
				break
			log("Waiting for data timed out! Re-sending START packet.")
			sock.sendto("START".encode(), (host, port))

		# strength, attention, meditation, delta, theta, low_alpha, high_alpha, low_beta, high_beta, low_gamma, high_gamma = data.split(",") #signal strength, attention, meditation, delta, theta, low alpha, high alpha, low beta, high beta, low gamma, high gamma
		'''
		strength = brain[0] # 0 good 200 bad
		attention = brain[1] # proprietary "attention"
		meditation = brain[2] # proprietary "meditation"
		delta = brain[3] # sleep
		theta = brain[4] # relaxed, meditative
		low_alpha = brain[5] # eyes closed, relaxed
		high_alpha = brain[6] # "wakeful relaxation, less depression and anxiety"
		low_beta = brain[7] # alert, focused
		high_beta = brain[8] # "significant stress, anxiety, paranoia"
		low_gamma = brain[9] # multi-sensory processing
		high_gamma = brain[10] # "happier, more receptive"
		'''
		'''
		log("\n"*50)
		if int(strength) != 0:
			log("Signal strength is not ideal; "+strength)
		log("Strength: "+strength)
		log("Attention: " + attention)
		log("Meditation: " + meditation)
		log("Delta: " + delta)
		log("Theta: " + theta)
		log("Low Alpha: " + low_alpha)
		log("High Alpha: " + high_alpha)
		log("Low Beta: " + low_beta)
		log("High Beta: " + high_beta)
		log("Low Gamma: " + low_gamma)
		log("High Gamma: " + high_gamma)
		'''


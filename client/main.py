import sys, time

from client import Client
from bernard import IA


host = "localhost"
port = 8080
team_name = "foo"

def	parse_arguments(arguments):
	pass

def	main():
	parse_arguments(sys.argv)

	client = Client(host, port, team_name)
	#connect return world's dimension
	wx, wy = client.connect()
	#time start, time, time ellapsed
	ts, t, te = 0, 0, 0
	#server sent by the server
	server_messages = []

	if wx > 0 and wy > 0:
		bernard = IA(team_name, wx, wy)
		ts = int(time.time() * 1000)
		while client.bot.alive == True:
			t = int(time.time() * 1000) - ts
			#bernard créé une cmd a transceive au serveur
			#il attend la réponse et actualise son état
			#brain lui permet de préparer des list de cmd à transceive
			#la list est executée cmd par cmd (create, transceive, receive
			bernard.interact(t, server_messages)
			client.transceive(bernard.brain.cmd)
			server_messages = client.receive(bernard.brain.cmd)
			te = int(time.time() * 1000) - t
	client.close()

if __name__ == "__main__":
	main()

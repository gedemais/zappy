import sys
import socket

from client import Client
from bernard import IA


host = "localhost"
port = 8080
team_name = "foo"

def	parse_arguments():
	arguments = sys.argv

if __name__ == "__main__":

	parse_arguments()

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client = Client(host, port, team_name, s)
	#connect return world's dimension
	wx, wy = client.connect()
	if wx > 0 and wy > 0:
		bernard = IA(wx, wy)
		bernard.fetch()
		ticks = 0
		while client.bot.alive == True:
			ticks = ticks + 1
			#bernard créé une cmd a transceive au serveur
			#il attend la réponse et actualise son état
			bernard.interact(ticks)
			client.transceive(bernard.brain.cmd)
			client.receive(bernard.brain.cmd)

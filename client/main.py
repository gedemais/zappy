import sys
import socket

from client import Client
from IA import IA
from brain import Brain


host = "localhost"
port = 8080
team_name = "foo"

def	parse_arguments():
	arguments = sys.argv

if __name__ == "__main__":

	parse_arguments()

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client = Client(host, port, team_name, s)
	client.connect()

	bernard = IA()

	while client.bot.alive == True:
		print("Busy:", bernard.interact(Brain.simpe()))
		client.transceive(bernard.cmd)
		client.receive(bernard.cmd)
import sys
import socket

from client import Client
from IA import IA


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

	bernard.square()
	bernard.prend()

	while client.bot.alive == True:
		client.bot.server_instructions(bernard.cmd)
		client.bot.callback(bernard.cmd)
		bernard.interact()
		client.bot.query(bernard.cmd)
		client.transceive(bernard.cmd)
		client.receive()

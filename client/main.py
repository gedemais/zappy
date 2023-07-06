import sys
import socket

from client import Client


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

	while client.bot.alive == True:
		client.receive()
		client.bot.run()
		client.transceive()

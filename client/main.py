import sys
import socket

from client import Client


host = "localhost"
port = 8080 # The same port as used by the server
team_name = "foo"

def	parse_arguments():
	arguments = sys.argv

moves = ['avance\n', 'avance\n', 'avance\n', 'avance\n', 'droite\n']

if __name__ == "__main__":

	parse_arguments()

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client = Client(host, port, team_name, s)
	client.connect_to_server()

	while True:
		client.broadcast("bonjour")
		for i in range(len(moves)):
			client.move(i)

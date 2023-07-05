import sys
import socket

from client import client

host = "localhost"
port = 8080 # The same port as used by the server
team_name = "foo"

moves = ['avance\n', 'avance\n', 'avance\n', 'avance\n', 'droite\n']

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def	send_cmd(cmd):
	s.send(bytes(cmd.encode("utf-8")))
	reply = s.recv(1024).decode("utf-8")
	print(cmd.strip(), reply.strip())

def	connect_to_server():
	s.connect((host, port))
	reply = s.recv(1024).decode("utf-8")
	print(reply) # BIENVENUE
	send_cmd(team_name)

def	parse_arguments():
	arguments = sys.argv

if __name__ == "__main__":

	parse_arguments()
	connect_to_server()

	while True:
		for i in range(len(moves)):
			send_cmd(moves[i])

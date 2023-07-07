import sys
import socket

from client import Client
from IA import IA
from brain import Brain

from time import sleep


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
    brain = Brain()
    brain.fetch()
    while client.bot.alive == True:
        brain.busy = bernard.interact(brain.update(client, bernard.cmd))
        client.transceive(bernard.cmd)
        client.receive(bernard.cmd)

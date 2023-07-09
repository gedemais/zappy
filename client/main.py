import sys
import socket

from client import Client
from brain import Brain
from bernard import IA

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
    bernard.fetch()

    ticks = 0
    while client.bot.alive == True:
        ticks = ticks + 1
        bernard.interact(ticks)
        client.transceive(bernard.brain.cmd)
        client.receive(bernard.brain.cmd)

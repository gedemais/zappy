# import random
import asyncio
import socket
import pygame
from pygame.locals import *

from gameBoard import *
from players import *
from ressource import Resource 
from constantes import *

host = socket.gethostname()
port = 8080 # The same port as used by the server
g = GameBoard()

async def handle_client(reader, writer):
    while True:
        data = await reader.read(1024)
        if not data:
            break

        commands = data.decode().split('\n')
        for command in commands:
            if command:
                g.process_input(command)

        writer.write(b"Data received and processed.\n")
        await writer.drain()

    writer.close()

async def main():
    try:
        # Replace 'server_host' and 'server_port' with the server's host and port
        reader, g.writer = await asyncio.open_connection(host, port)

        # Start handling input from the server and sending commands
        input_task = asyncio.create_task(handle_client(reader))

        # Wait for both tasks to complete
        await input_task

    except ConnectionError:
        print("Connection to the server failed.")
                
    # another way of initializing a connection
    # s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # s.connect((host, port))
    # reply = s.recv(1024).decode('utf-8')
    # print(reply) # BIENVENUE
    # s.send(bytes(init.encode('utf-8')))
    # reply = s.recv(1024).decode('utf-8')
    # print(reply)
        
if __name__ == "__main__":
    asyncio.run(main())

import pygame
import socket
from Renderer import Renderer
from Connector import Connector
from World import World


host = socket.gethostname()
port = 8080                   # The same port as used by the server
connector = Connector(host, port)
response = connector.authenticate()

world = World(response)

connector.tick = world.t

renderer = Renderer(world, tile_size=100)

cycle = 0

# Main game loop
while renderer.is_running:

    renderer.process_events()
    connector.process(world)
    renderer.render(world)

    cycle += 1

# Quit Pygame
pygame.quit()

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

renderer = Renderer(world, tile_size=50, loot_scale=0.75)

# Main game loop
while renderer.is_running:

    renderer.process_events()
    renderer.render(world)
    connector.process(world)

# Quit Pygame
pygame.quit()

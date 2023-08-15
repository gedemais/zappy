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

cycle = 0

# Main game loop
while renderer.is_running:

    renderer.process_events()
    connector.process(world)
    renderer.render(world)

    for team in world.teams.items():
        team = team[1]
        for player in team.players.keys():
            pl = team.players[player]
            print(pl.x, pl.y)

    if cycle % 60 == 0:
        for y in range(world.map_height):
            for x in range(world.map_width):
                for i in range(6):
                    world.map[y][x][i] -= 1 if world.map[y][x][i] > 0 else 0


    cycle += 1

# Quit Pygame
pygame.quit()

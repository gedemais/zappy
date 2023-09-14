import pygame
import socket
from Renderer import Renderer
from Connector import Connector
from World import World
from Team import Team
import json
import os

host = socket.gethostname()
port = 8080                   # The same port as used by the server

connector = Connector(host, port)
response = connector.authenticate()

world = World(response, connector)

connector.tick = world.t

renderer = Renderer(world, tile_size=50)

hud_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
hud_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
hud_socket.bind((socket.gethostbyname(host), 4242))

os.system('python3 hud.py &')

print('Waiting for HUD connection...')
hud_socket.listen()
hud, hud_address = hud_socket.accept()
print('HUD connected !')
hud_connected = True

game_ended = False
cycle = 0

# Main game loop
while renderer.is_running:

    renderer.process_events()
    res = connector.process(world)
    renderer.render(world, game_ended)

    if res != 'done':
        message = 'GW {}'.format(res)
        hud.send(bytes(message.encode('utf-8')))
        game_ended = True
        renderer.winner = res

    if hud_connected and game_ended == False and connector.ready and cycle % world.t == 0:
        teams = [team[1].to_dict(renderer, world) for team in world.teams.items()]

        message = json.dumps(teams)
        try:
            hud.send(bytes(message.encode('utf-8')))
        except:
            hud_connected = False
            print('HUD disconnected !')

    cycle += 1

# Quit Pygame
pygame.quit()

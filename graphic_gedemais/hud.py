from sys import argv
import pygame
import socket
import json
from time import sleep

def render_food_circles(player, y):
    food_stock = player['inventory'][0]
    if food_stock >= 30:
        color = (0, 255, 0)
    elif food_stock >= 20:
        color = (255, 160, 0)
    elif food_stock >= 10:
        color = (255, 80, 0)
    else:
        color = (255, 0, 0)

    for i in range(food_stock):
        pygame.draw.circle(window, color, (i * 5 + 50, y), 3)

    text = numbers_font.render(str(food_stock), True, (255, 255, 255), bgd)
    rect = text.get_rect()
    rect.center = (25, y)
    window.blit(text, rect)

def render_level_bar(player, y):
    level = player['level']

    pygame.draw.rect(window, (255, 255, 255), (400, y - 5, 202, 12))
    pygame.draw.rect(window, bgd, (401, y + 1 - 5, 200, 10))

    text = numbers_font.render(str(level), True, (255, 255, 255), bgd)
    rect = text.get_rect()
    rect.center = (380, y + 1)
    window.blit(text, rect)


    for i in range(level):
        color = (255, (255 - i * 30), 0)
        pygame.draw.rect(window, color, (401 + i * 25, y + 1 - 5, 25, 10))




def render_team(bgd, index, row_size, name, players):

    off_y = index * row_size

    title = title_font.render('Team ' + name, True, (255, 255, 255), bgd)
    rect = title.get_rect()
    rect.center = (win_width / 2, off_y + 32)
    window.blit(title, rect)

    sprite = sprites[0]
    rect = sprite.get_rect()
    rect.center = (win_width / 2, off_y + row_size / 2)
    window.blit(sprite, rect)

    n = len(players)
    s = '{} player'.format(len(players))
    s += 's' if n > 1 else ''
    title = subtitles_font.render(s, True, (255, 255, 255), bgd)
    rect = title.get_rect()
    rect.center = (win_width / 2, off_y + 32 + row_size - row_size / 3)
    window.blit(title, rect)

    food = subtitles_font.render('Food', True, (255, 255, 255), bgd)
    rect = food.get_rect()
    rect.center = (win_width / 5, off_y + 32)
    window.blit(food, rect)

    levels = subtitles_font.render('Levels', True, (255, 255, 255), bgd)
    rect = levels.get_rect()
    rect.center = (win_width - win_width / 5, off_y + 32)
    window.blit(levels, rect)

    for p, player in enumerate(players):
        y = off_y + 60 + p * 20
        render_food_circles(player, y)
        render_level_bar(player, y)


# Initialize Pygame
pygame.init()
is_running = True

title_font = pygame.font.Font('ChrustyRock.ttf', 32)
subtitles_font = pygame.font.Font('ChrustyRock.ttf', 20)
numbers_font = pygame.font.Font('JosefinSans.ttf', 20)

sleep(2)

host = socket.gethostname()
port = 4242

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.connect((socket.gethostbyname(host), port))

request = s.recv(65536).decode('utf-8')
if request is not None:
    teams = json.loads(request)

nb_teams = len(teams)

# Window dimensions
row_size = 180

win_width = 630
win_height = nb_teams * row_size

# Window creation
window = pygame.display.set_mode((win_width, win_height))

# Window caption
pygame.display.set_caption("Zappy HUD")

# Clock initialization
clock = pygame.time.Clock()
fps = 1

path = './sprites/spritesheets/'
sprites =   [
                pygame.transform.scale(pygame.image.load(path + "chainSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "darkmaleSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "elvenSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "goldenarmorSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "leatherSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "plateSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "platefemaleSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2)),
                pygame.transform.scale(pygame.image.load(path + "rabbitSpriteSheet.png").subsurface((1, 641, 62, 62)), (row_size / 2, row_size / 2))
            ]

while is_running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False


    request = s.recv(65536).decode('utf-8')
    if request is not None:
        teams = json.loads(request)

    for i, team in enumerate(teams):
        bgd = ((i + 1) * 20, (i + 1) * 20, (i + 1) * 20)
        pygame.draw.rect(window, bgd, (0, i * row_size, win_width, row_size))
        if 'name' in team.keys():
            render_team(bgd, i, row_size, team['name'], team['players'])


    pygame.display.flip()
    clock.tick(fps)



pygame.quit()

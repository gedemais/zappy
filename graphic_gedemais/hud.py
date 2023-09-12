from sys import argv
import pygame
import socket
import json
from time import sleep

is_rendered = []

def render_food_circles(player, y, bgd):
    food_stock = player['inventory'][0]

    for i in range(food_stock if food_stock <= 42 else 42):
        color = ((255 - i * 6), i * 6, 0)
        pygame.draw.circle(window, color, (i * 5 + 50, y), 3)

    text = numbers_font.render(str(food_stock), True, (255, 255, 255), bgd)
    rect = text.get_rect()
    rect.center = (25, y)
    window.blit(text, rect)

def render_level_bar(player, y, bgd):
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




def render_team(index, rendereds, row_size, name, players, render):

    off_y = rendereds * row_size + ((index - rendereds) * row_size / 4)

    bgd = ((index % 8 + 1) * 20, (index % 8 + 1) * 20, (index % 8 + 1) * 20)
    pygame.draw.rect(window, bgd, (0, off_y, win_width, row_size if render else row_size / 4))

    if render:
        title = title_font.render('Team ' + name, True, (255, 255, 255), bgd)
    else:
        title = subtitles_font.render('Team ' + name, True, (255, 255, 255), bgd)
    rect = title.get_rect()
    rect.center = (win_width / 2, off_y + 32)
    window.blit(title, rect)

    if render == False:
        return

    sprite = sprites[index % 8]
    rect = sprite.get_rect()
    rect.center = (win_width / 2, off_y + row_size / 2)
    window.blit(sprite, rect)

    n = len(players)
    s = '{} player'.format(len(players))
    s += 's' if n > 1 else ''
    title = numbers_font.render(s, True, (255, 255, 255), bgd)
    rect = title.get_rect()
    rect.center = (win_width / 2, off_y + 40 + row_size - row_size / 3)
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
        render_food_circles(player, y, bgd)
        render_level_bar(player, y, bgd)




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
fps = 5

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

def handle_click(pos_y):
    off_y = 0
    for i in range(len(is_rendered)):
        off_y += row_size / (1 if is_rendered[i] else 4)
        if off_y >= pos_y:
            is_rendered[i] = False if is_rendered[i] else True
            return


i = 0
toggle_all = False
while is_running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            is_running = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                if abs(event.pos[0] - (win_width / 2)) < 100:
                    handle_click(event.pos[1])
        elif event.type == pygame.KEYUP:

            if event.key == pygame.K_SPACE:
                for i in range(len(is_rendered)):
                    is_rendered[i] = toggle_all
                toggle_all = False if toggle_all else True


    if i % fps == 0:
        request = s.recv(65536).decode('utf-8')
        if request is not None:
            teams = json.loads(request)

    pygame.draw.rect(window, (0, 0, 0), (0, 0, win_width, win_height))

    rendereds = 0
    for i, team in enumerate(teams):
        if i >= len(is_rendered):
            is_rendered.append(False)

        if 'name' in team.keys():
            render_team(i, rendereds, row_size, team['name'], team['players'], is_rendered[i])

        if is_rendered[i]:
            rendereds += 1

    pygame.display.flip()
    clock.tick(fps)



pygame.quit()

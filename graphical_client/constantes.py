import pygame
from enum import Enum

# Set up the display
WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

MAP_WIDTH = 10
MAP_HEIGHT = 10

FPS = 60

# Set up colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
BROWN = (146, 109, 39)

# Event for collecting resources
COLLECT_RESOURCE_EVENT = pygame.USEREVENT + 1

# Enum for player orientation
class Orientation(Enum):
    N = 1
    E = 2
    S = 3
    O = 4

# Define level_up recipes
recipes = [
    # level 1->2
    {
        'linemate': 1,
        'deraumere': 0,
        'sibur': 0,
        'mendiane': 0,
        'phiras': 0,
        'thystame': 0 
    },
    # level 2->3
    {
        'linemate': 1,
        'deraumere': 1,
        'sibur': 1,
        'mendiane': 0,
        'phiras': 0,
        'thystame': 0 
    },
    # level 3->4
    {
        'linemate': 2,
        'deraumere': 0,
        'sibur': 1,
        'mendiane': 0,
        'phiras': 2,
        'thystame': 0 
    },
    # level 4->5
    {
        'linemate': 1,
        'deraumere': 1,
        'sibur': 2,
        'mendiane': 0,
        'phiras': 1,
        'thystame': 0 
    },
    # level 5->6
    {
        'linemate': 1,
        'deraumere': 2,
        'sibur': 1,
        'mendiane': 3,
        'phiras': 0,
        'thystame': 0 
    },
    # level 6->7
    {
        'linemate': 1,
        'deraumere': 2,
        'sibur': 3,
        'mendiane': 0,
        'phiras': 1,
        'thystame': 0 
    },
    # level 7->8
    {
        'linemate': 2,
        'deraumere': 2,
        'sibur': 2,
        'mendiane': 2,
        'phiras': 2,
        'thystame': 1 
    }
]
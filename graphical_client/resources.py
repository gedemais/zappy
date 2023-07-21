import pygame

from resource import Resource

class Linemate(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'linemate'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Deraumere(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'deraumere'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Sibur(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'sibur'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Mendiane(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'mendiane'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Phiras(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'phiras'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Thystame(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'thystame'
        self.image = pygame.image.load("spritesUsed/minerals.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()

class Nourriture(Resource):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'nourriture'
        self.image = pygame.image.load("spritesUsed/nourriture.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60)).convert()
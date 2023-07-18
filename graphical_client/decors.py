import pygame

from decor import Decor

class GreenTree(Decor):
    def __init__(self, x, y):
        super().__init__(x, y)

        self.name = 'greenTree'
        self.image = pygame.image.load("spritesUsed/Trees.png").convert_alpha().subsurface(pygame.Rect(60, 0, 60, 60))

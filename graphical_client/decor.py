import pygame
from pygame.locals import *

class Decor(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()

        # Define decor's position
        self.position = (x, y)
        # self.rect = self.image.get_rect()
        # self.rect.x = x
        # self.rect.y = y


    def update(self):
        # Do something with decor if needed
        pass

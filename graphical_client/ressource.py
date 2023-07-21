import pygame
from pygame.locals import *

class Resource(pygame.sprite.Sprite):
    def __init__(self, x, y):
        super().__init__()

        # Define resource's position
        self.position = (x, y)
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        
        self.quantity = 0

        # Set timer for resource disappearance
        self.timer = pygame.time.get_ticks()
        self.collecting = False

        # Set the player collecting the resource
        self.collector = None

    def update(self):
        # Check if resource should disappear
        if pygame.time.get_ticks() - self.timer > 2000:  # 5000 milliseconds = 5 seconds
            self.collecting = False
            self.collector.resources[self.name] += 1
            self.kill()
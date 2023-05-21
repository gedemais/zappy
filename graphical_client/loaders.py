import pygame

from loader import PlayerLoader

class RabbitPlayerLoader(PlayerLoader):
    def __init__(self):
        super().__init__()

        # contain sprite sheet 
        self.image = pygame.image.load("spritesUsed/rabbitSpriteSheet.png").convert_alpha()

        # call method to load all the animation
        self.load_animations()

# Add new character loader here (differents sprites), based on Rabbit Player Loader model.
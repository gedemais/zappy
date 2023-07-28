import pygame

class   Team():
    def __init__(self, name):
        self.name = name
        self.players = []
        self.eggs = []

        # Have to build a mechanism to avoid choosing the same spritesheet for all teams
        animations = pygame.image.load('./sprites/rabbitSpriteSheet.png')
        self.players_sprites = [
                    animations.subsurface((0, 832, 64, 64)),
                    animations.subsurface((0, 704, 64, 64)),
                    animations.subsurface((0, 576, 64, 64)),
                    animations.subsurface((0, 640, 64, 64))
                ]


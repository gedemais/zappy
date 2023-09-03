import pygame
import numpy as np
from Player import S

class   Renderer():

    def __init__(self, world, tile_size=50, loot_scale=0.75):
    # Initialize Pygame
        pygame.init()
        self.is_running = True

    # Window initialization
        # Map dimensions
        self.map_width = world.map_width
        self.map_height = world.map_height

        # Window dimensions
        self.win_width = self.map_width * tile_size + self.map_width
        self.win_height = self.map_height * tile_size + self.map_height
        self.tile_size = tile_size
        self.loot_scale = loot_scale

        # Window creation
        self.window = pygame.display.set_mode((self.win_width, self.win_height))

        # Window caption
        pygame.display.set_caption("Zappy Visualiser")

        # Clock initialization
        self.clock = pygame.time.Clock()
        self.fps = world.t

        # Resources loading
        self.load_resources()


    def process_events(self):
        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.is_running = False


    def load_resources(self):
        # Background resource loading
        self.bgd_tile = pygame.image.load('./sprites/grassFlower.jpeg')
        self.bgd_tile = pygame.transform.scale(self.bgd_tile, (self.tile_size, self.tile_size))
        self.bgd_tile_rect = self.bgd_tile.get_rect()
        self.generate_background()

        eggs = pygame.image.load('./sprites/eggs.jpg')
        minerals = pygame.image.load('./sprites/minerals.png')
        food = pygame.image.load('./sprites/minerals.png')
        self.loot_images = [
                food.subsurface((100, 0, 140, 140)),
                minerals.subsurface((0, 0, 28, 28)),
                minerals.subsurface((115, 0, 28, 28)),
                minerals.subsurface((230, 0, 28, 28)),
                minerals.subsurface((0, 230, 28, 28)),
                minerals.subsurface((115, 115, 28, 28)),
                minerals.subsurface((230, 115, 28, 28)),
                ]

        self.eggs_images = [
                eggs.subsurface((50, 0, 250, 200)),
                ]


        # Have to build a mechanism to avoid choosing the same spritesheet for all teams
        animations = pygame.image.load('./sprites/rabbitSpriteSheet.png')

        # IDLE animations
        self.player_animations = {
                    'idle_north' : [pygame.transform.scale(animations.subsurface((0, 512, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_west' : [pygame.transform.scale(animations.subsurface((0, 576, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_south' : [pygame.transform.scale(animations.subsurface((0, 640, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_east' : [pygame.transform.scale(animations.subsurface((0, 704, 64, 64)), (self.tile_size, self.tile_size))]
                }

        # Walking animations
        self.player_animations['walking_north'] = []
        self.player_animations['walking_west'] = []
        self.player_animations['walking_south'] = []
        self.player_animations['walking_east'] = []

        for i in range(1, 9):
            x = i * 64
            self.player_animations['walking_north'].append(pygame.transform.scale(animations.subsurface((x, 512, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['walking_west'].append(pygame.transform.scale(animations.subsurface((x, 576, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['walking_south'].append(pygame.transform.scale(animations.subsurface((x, 640, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['walking_east'].append(pygame.transform.scale(animations.subsurface((x, 704, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations['kick_north'] = []
        self.player_animations['kick_west'] = []
        self.player_animations['kick_south'] = []
        self.player_animations['kick_east'] = []




    def render_loot(self, world, x, y):
        coords =    [
                        (0, 0),
                        (0, 1),
                        (0, 2),

                        (1, 0),
                        (1, 2),
                        
                        (2, 0),
                        (2, 1),
                        (2, 2),
                    ]

        for i in range(1, 7):
            if world.map[y][x][i] > 0:
                loot_x = x * self.tile_size + coords[i][0] * self.loot_tile_size + x
                loot_y = y * self.tile_size + coords[i][1] * self.loot_tile_size + y

                scale = (1.0 - self.loot_scale) * (self.loot_tile_size / 2.0)

                loot_x += scale
                loot_y += scale

                self.window.blit(self.images[i], (loot_x, loot_y, scale, scale))


    def generate_background(self):
        self.background = pygame.Surface((self.win_width, self.win_height))
        self.background.fill((0, 0, 0))
        for x in range(self.map_width):
            for y in range(self.map_height):
                self.bgd_tile_rect[0] = x * self.tile_size + x
                self.bgd_tile_rect[1] = y * self.tile_size + y
                self.bgd_tile_rect[2] = self.bgd_tile_rect[0] + self.tile_size + x
                self.bgd_tile_rect[3] = self.bgd_tile_rect[1] + self.tile_size + y
                self.background.blit(self.bgd_tile, self.bgd_tile_rect)


    def get_animation(self, player):
        keys = ['north', 'east', 'south', 'west']
        if player.state == S.IDLE:
            return self.player_animations['idle_' + keys[player.o]]

        elif player.state.value[0] >= S.WALKING_NORTH.value[0]:
            if player.state.value[0] <= S.WALKING_WEST.value[0]:
                return self.player_animations['walking_' + keys[player.o]]


    def render_players(self, world):

        for team in world.teams.items():
            team = team[1]
            for player in team.players.items():
                player = player[1]
                animation = self.get_animation(player)

                off_x, off_y = 0, 0

                if player.state == S.WALKING_NORTH:
                    off_y = self.tile_size / len(animation) * player.step
                if player.state == S.WALKING_SOUTH:
                    off_y = -self.tile_size / len(animation) * player.step
                if player.state == S.WALKING_WEST:
                    off_x = -self.tile_size / len(animation) * player.step
                elif player.state == S.WALKING_EAST:
                    off_x = self.tile_size / len(animation) * player.step

                off_x *= 1.5
                off_y *= 1.5

                self.window.blit(animation[player.step], (player.x * self.tile_size + player.x + off_x, player.y * self.tile_size + player.y + off_y))

                player.step += 1

                if player.step == len(animation):
                    player.state = S.IDLE
                    player.step = 0


    def render(self, world):
        self.loot_tile_size = self.tile_size / 3
        self.images = self.loot_images
        size = self.loot_tile_size * self.loot_scale

        for i, img in enumerate(self.images):
            self.images[i] = pygame.transform.scale(img, (size, size))

        self.window.blit(self.background, self.background.get_rect())

        for y in range(self.map_height):
            for x in range(self.map_width):
                self.render_loot(world, x, y)
                #self.render_egg(world, x, y)

        self.render_players(world)

        pygame.display.flip()
        self.clock.tick(self.fps)

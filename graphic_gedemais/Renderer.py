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
        broadcast_animation = pygame.image.load('./sprites/broadcast.png')
        broadcast_animation.set_colorkey((0, 0, 0))
        minerals = pygame.image.load('./sprites/minerals.png')
        food = pygame.image.load('./sprites/apple.png')

        self.loot_images = [
                food,
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
        animations = pygame.image.load('./sprites/spritesheets/rabbitSpriteSheet.png')

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

        self.player_animations['pushing_north'] = []
        self.player_animations['pushing_west'] = []
        self.player_animations['pushing_south'] = []
        self.player_animations['pushing_east'] = []

        for i in range(8):
            x = i * 64
            self.player_animations['pushing_north'].append(pygame.transform.scale(animations.subsurface((x, 256, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['pushing_west'].append(pygame.transform.scale(animations.subsurface((x, 320, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['pushing_south'].append(pygame.transform.scale(animations.subsurface((x, 384, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations['pushing_east'].append(pygame.transform.scale(animations.subsurface((x, 448, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations['takeput'] = []

        for i in range(4):
            x = i * 64
            self.player_animations['takeput'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size))),

        self.player_animations['takeput'].append(self.player_animations['takeput'][2])
        self.player_animations['takeput'].append(self.player_animations['takeput'][1])
        self.player_animations['takeput'].append(self.player_animations['takeput'][0])

        self.broadcast_animation = []
        print(broadcast_animation)
        for i in range(7):
            x = i * 64
            self.broadcast_animation.append(pygame.transform.scale(broadcast_animation.subsurface((x, 0, 64, 64)), (self.tile_size / 2, self.tile_size / 2)))

        incantating_animation = []
        for i in range(7):
            x = i * 64
            incantating_animation.append(pygame.transform.scale(animations.subsurface((x, 128, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations['incantation'] = []
        sprite = 0
        for i in range(300):
            self.player_animations['incantation'].append(incantating_animation[sprite])
            if i % 50 == 0:
                sprite += 1

        self.player_animations['laying'] = []
        for i in range(4):
            x = i * 64
            self.player_animations['laying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations['laying'].append(self.player_animations['laying'][2])
        self.player_animations['laying'].append(self.player_animations['laying'][1])
        self.player_animations['laying'].append(self.player_animations['laying'][0])

        for i in range(5):
            self.player_animations['laying'] += self.player_animations['laying']

        self.player_animations['dying'] = []
        for i in range(7):
            x = i * 64
            self.player_animations['dying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))

        x = 5 * 64
        for i in range(35):
            self.player_animations['dying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))


        

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

        for i in range(7):
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
        walking_states =   [
                        S.WALKING_NORTH,
                        S.WALKING_EAST,
                        S.WALKING_SOUTH,
                        S.WALKING_WEST
                    ]

        #print('state :', player.state)
        if player.state == S.IDLE or player.state == S.BROADCASTING:
            return self.player_animations['idle_' + keys[player.o]]


        if player.state in walking_states:
                return self.player_animations['walking_' + keys[player.o]]

        if player.state == S.PUSHING:
            return self.player_animations['pushing_' + keys[player.o]]

        if player.state == S.TAKING or player.state == S.PUTTING:
            return self.player_animations['takeput']

        if player.state == S.INCANTATING:
            return self.player_animations['incantation']

        if player.state == S.LAYING_EGG:
            return self.player_animations['laying']

        if player.state == S.DYING:
            return self.player_animations['dying']

        print('ANIMATION NOT FOUND')
        assert(False)


    def render_players(self, world):

        for team in world.teams.items():
            team = team[1]
            for player in team.players.items():
                player = player[1]

                animation = self.get_animation(player)


                if player.step == len(animation):
                    if player.state != S.BROADCASTING or player.step == 7:
                        if len(player.states_queue) > 0:
                            player.state = player.states_queue[0]
                            player.states_queue.pop(0)
                        else:
                            player.state = S.IDLE

                        player.step = 0

                print(player.state)

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

                if player.state == S.BROADCASTING:
                    sprite = animation[0]
                else:
                    sprite = animation[player.step]

                self.window.blit(sprite, (player.x * self.tile_size + player.x + off_x, player.y * self.tile_size + player.y + off_y))

                if player.state == S.BROADCASTING:
                    animation = self.broadcast_animation
                    off_x += self.tile_size / 2
                    self.window.blit(animation[player.step], (player.x * self.tile_size + player.x + off_x, player.y * self.tile_size + player.y + off_y))

                player.step += 1


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

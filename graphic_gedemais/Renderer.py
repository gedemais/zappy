import pygame, random
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

    def load_spritesheet(self, broadcast_animation, team_index):
        self.spritesheet_paths = [
                                "goldenarmorSpriteSheet.png",
                                "elvenSpriteSheet.png",
                                "chainSpriteSheet.png",
                                "darkmaleSpriteSheet.png",
                                "leatherSpriteSheet.png",
                                "plateSpriteSheet.png",
                                "platefemaleSpriteSheet.png",
                                "rabbitSpriteSheet.png"
                            ]

        animations = pygame.image.load('./sprites/spritesheets/' + self.spritesheet_paths[team_index % 8])

        # IDLE animations
        self.player_animations[team_index % 8] = {
                    'idle_north' : [pygame.transform.scale(animations.subsurface((0, 512, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_west' : [pygame.transform.scale(animations.subsurface((0, 576, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_south' : [pygame.transform.scale(animations.subsurface((0, 640, 64, 64)), (self.tile_size, self.tile_size))],
                    'idle_east' : [pygame.transform.scale(animations.subsurface((0, 704, 64, 64)), (self.tile_size, self.tile_size))]
                }

        # Walking animations
        self.player_animations[team_index % 8]['walking_north'] = []
        self.player_animations[team_index % 8]['walking_west'] = []
        self.player_animations[team_index % 8]['walking_south'] = []
        self.player_animations[team_index % 8]['walking_east'] = []

        for i in range(1, 9):
            x = i * 64
            self.player_animations[team_index % 8]['walking_north'].append(pygame.transform.scale(animations.subsurface((x, 512, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['walking_west'].append(pygame.transform.scale(animations.subsurface((x, 576, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['walking_south'].append(pygame.transform.scale(animations.subsurface((x, 640, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['walking_east'].append(pygame.transform.scale(animations.subsurface((x, 704, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations[team_index % 8]['pushing_north'] = []
        self.player_animations[team_index % 8]['pushing_west'] = []
        self.player_animations[team_index % 8]['pushing_south'] = []
        self.player_animations[team_index % 8]['pushing_east'] = []

        for i in range(8):
            x = i * 64
            self.player_animations[team_index % 8]['pushing_north'].append(pygame.transform.scale(animations.subsurface((x, 256, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['pushing_west'].append(pygame.transform.scale(animations.subsurface((x, 320, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['pushing_south'].append(pygame.transform.scale(animations.subsurface((x, 384, 64, 64)), (self.tile_size, self.tile_size))),
            self.player_animations[team_index % 8]['pushing_east'].append(pygame.transform.scale(animations.subsurface((x, 448, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations[team_index % 8]['takeput'] = []

        for i in range(4):
            x = i * 64
            self.player_animations[team_index % 8]['takeput'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size))),

        self.player_animations[team_index % 8]['takeput'].append(self.player_animations[team_index % 8]['takeput'][2])
        self.player_animations[team_index % 8]['takeput'].append(self.player_animations[team_index % 8]['takeput'][1])
        self.player_animations[team_index % 8]['takeput'].append(self.player_animations[team_index % 8]['takeput'][0])
        self.broadcast_animation = []
        print(broadcast_animation)
        for i in range(7):
            x = i * 64
            self.broadcast_animation.append(pygame.transform.scale(broadcast_animation.subsurface((x, 0, 64, 64)), (self.tile_size / 2, self.tile_size / 2)))

        incantating_animation = []
        for i in range(7):
            x = i * 64
            incantating_animation.append(pygame.transform.scale(animations.subsurface((x, 128, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations[team_index % 8]['incantation'] = []
        sprite = 0
        for i in range(300):
            self.player_animations[team_index % 8]['incantation'].append(incantating_animation[sprite])
            if i % 50 == 0:
                sprite += 1

        self.player_animations[team_index % 8]['laying'] = []
        for i in range(4):
            x = i * 64
            self.player_animations[team_index % 8]['laying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))

        self.player_animations[team_index % 8]['laying'].append(self.player_animations[team_index % 8]['laying'][2])
        self.player_animations[team_index % 8]['laying'].append(self.player_animations[team_index % 8]['laying'][1])
        self.player_animations[team_index % 8]['laying'].append(self.player_animations[team_index % 8]['laying'][0])

        for i in range(5):
            self.player_animations[team_index % 8]['laying'] += self.player_animations[team_index % 8]['laying']

        self.player_animations[team_index % 8]['dying'] = []
        for i in range(7):
            x = i * 64
            self.player_animations[team_index % 8]['dying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))

        x = 5 * 64
        for i in range(35):
            self.player_animations[team_index % 8]['dying'].append(pygame.transform.scale(animations.subsurface((x, 1280, 64, 64)), (self.tile_size, self.tile_size)))




    def load_resources(self):
        # Background resource loading
        self.bgd_tile = pygame.image.load('./sprites/grassFlower.jpeg')
        self.bgd_tile = pygame.transform.scale(self.bgd_tile, (self.tile_size, self.tile_size))
        self.bgd_tile_rect = self.bgd_tile.get_rect()
        self.generate_background()

        eggs = pygame.image.load('./sprites/eggs.png')
        self.egg = pygame.transform.scale(eggs, (self.tile_size / 2, self.tile_size / 2))
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

        self.player_animations = []
        for i in range(8):
            self.player_animations.append({})
            self.load_spritesheet(broadcast_animation, i)




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

    def render_eggs(self, world):
        for t in world.teams.items():
            for egg in t[1].eggs.items():
                x = egg[1].x
                y = egg[1].y
                rect = [0, 0, 0, 0]
                off_x = self.tile_size / 4
                off_y = self.tile_size / 4
                rect[0] = x * self.tile_size + x + off_x
                rect[1] = y * self.tile_size + y + off_y
                rect[2] = self.bgd_tile_rect[0] + self.tile_size + x + off_x
                rect[3] = self.bgd_tile_rect[1] + self.tile_size + y + off_y
                self.window.blit(self.egg, rect)

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
            return self.player_animations[player.team_index % 8]['idle_' + keys[player.o]]


        if player.state in walking_states:
                return self.player_animations[player.team_index % 8]['walking_' + keys[player.o]]

        if player.state == S.PUSHING:
            return self.player_animations[player.team_index % 8]['pushing_' + keys[player.o]]

        if player.state == S.TAKING or player.state == S.PUTTING:
            return self.player_animations[player.team_index % 8]['takeput']

        if player.state == S.INCANTATING:
            return self.player_animations[player.team_index % 8]['incantation']

        if player.state == S.LAYING_EGG:
            return self.player_animations[player.team_index % 8]['laying']

        if player.state == S.DYING:
            return self.player_animations[player.team_index % 8]['dying']

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
                            print(player.state)
                            player.states_queue.pop(0)
                        else:
                            player.state = S.IDLE

                        player.step = 0

                #print(player.state)

                off_x, off_y = 0, 0

                if player.state == S.WALKING_NORTH:
                    off_y = self.tile_size / len(animation) * player.step
                    off_y *= -1 if player.y == 0 and player.next_y == world.map_height - 1 else 1
                if player.state == S.WALKING_SOUTH:
                    off_y = -self.tile_size / len(animation) * player.step
                    off_y *= -1 if player.y == world.map_height - 1 and player.next_y == 0 else 1
                if player.state == S.WALKING_WEST:
                    off_x = -self.tile_size / len(animation) * player.step
                    off_x *= -1 if player.x == world.map_width - 1 and player.next_x == 0 else 1
                elif player.state == S.WALKING_EAST:
                    off_x = self.tile_size / len(animation) * player.step
                    off_x *= -1 if player.x == 0 and player.next_x == world.map_width - 1 else 1

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
                    player.step = player.step if player.step < len(animation) else 6
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

        self.render_eggs(world)
        self.render_players(world)

        pygame.display.flip()
        self.clock.tick(self.fps)

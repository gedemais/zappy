import pygame
import numpy as np

class   Renderer():

    def __init__(self, world, tile_size=50, loot_scale=1.0):
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


    def greyscale(self, surface):
        arr = pygame.surfarray.pixels3d(surface)
        mean_arr = np.dot(arr[:,:,:], [0.216, 0.587, 0.144])
        mean_arr3d = mean_arr[..., np.newaxis]
        new_arr = np.repeat(mean_arr3d[:, :, :], 3, axis=2)
        return pygame.surfarray.make_surface(new_arr)


    def load_resources(self):
        # Background resource loading
        self.bgd_tile = pygame.image.load('./sprites/grassFlower.jpeg')
        self.bgd_tile = pygame.transform.scale(self.bgd_tile, (self.tile_size, self.tile_size))
        self.bgd_tile_rect = self.bgd_tile.get_rect()
        self.generate_background()

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

        self.player_animations = {
                'walking_north' : (64, 576, 512, 64),
                'walking_south' : (64, 704, 512, 64),
                'walking_east' : (64, 832, 512, 64),
                'walking_west' : (64, 640, 512, 64)
                }


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


    def render_players(self, world):

        for team in world.teams.keys():
            t = world.teams[team]
            for player in t.players:
                self.window.blit(t.players_sprites[player.o], (player.x * self.tile_size, player.y * self.tile_size))



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

        self.render_players(world)

        pygame.display.flip()
        self.clock.tick(self.fps)

import pygame

class   Renderer():

    def __init__(self, world, tile_size=50, fps=60):
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

        # Window creation
        self.window = pygame.display.set_mode((self.win_width, self.win_height))

        # Window caption
        pygame.display.set_caption("Zappy Visualiser")

        # Clock initialization
        self.clock = pygame.time.Clock()
        self.fps = fps

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


    def render(self):
        self.window.blit(self.background, self.background.get_rect())
        pygame.display.flip()
        self.clock.tick(self.fps)

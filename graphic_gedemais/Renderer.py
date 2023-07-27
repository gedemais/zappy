import pygame

class   Renderer():

    def __init__(self, map_width, map_height):
        # Initialize Pygame
        pygame.init()
        self.is_running = True

        # Window initialization
        self.window = pygame.display.set_mode((map_width, map_height))

        # Window caption
        pygame.display.set_caption("Zappy Visualiser")

        self.clock = pygame.time.Clock()

    def process_events(self):
        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self.is_running = False

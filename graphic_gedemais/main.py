import pygame
from Renderer import Renderer


# Window dimensions
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

renderer = Renderer(WINDOW_WIDTH, WINDOW_HEIGHT)

# Main game loop
while renderer.is_running:

    renderer.process_events()
    # Clear the screen with a white background
    renderer.window.fill((255, 255, 255))

    # Draw your game objects here (e.g., using pygame.draw functions)

    # Update the display
    pygame.display.flip()

    # Control the frame rate (e.g., 60 frames per second)
    renderer.clock.tick(60)

# Quit Pygame
pygame.quit()


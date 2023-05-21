import random
import pygame
from pygame.locals import *

from gameBoard import *
from players import *
from ressource import Resource 
from constantes import *

pygame.init()

window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("El Collector")

def addResources(resources):
    # Add resources to the resources group
    # Example: resources.add(Resource(100, 100))

    # Only for debug purpose
    resources.add(Resource(200, 100))
    resources.add(Resource(100, 300))
    resources.add(Resource(450, 200))
    resources.add(Resource(550, 550))
    resources.add(Resource(300, 300))    

# def updateResourcesCollected(player, resources):
#     # Check for resource collection
#     collected_resources = pygame.sprite.spritecollide(player, resources, True)
#     for resource in collected_resources:
#         # Increase player's resource count

#         resource.kill()
#         pass

def draw(window, all_sprites, resources):
    # Draw everything
    window.fill(BROWN)
    all_sprites.draw(window)
    resources.draw(window)
    pygame.display.flip()

def main():
    clock = pygame.time.Clock()


    game_board = GameBoard(WINDOW_WIDTH, WINDOW_HEIGHT)
    player = RabbitPlayer(game_board, 0, 0, 50, 50)
    all_sprites = pygame.sprite.Group(player)
    resources = pygame.sprite.Group()
    addResources(resources)

    # Main game loop
    running = True
    while running:
        # Limit the frame rate
        clock.tick(FPS)

        # Event handling
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            elif event.type == COLLECT_RESOURCE_EVENT:
                collisions = pygame.sprite.spritecollide(player, resources, True)
                for collision in collisions:
                    if collision.collecting:
                        player.resources += collision.value
                        collision.collecting = False
                        collision.kill()
            elif event.type == pygame.KEYDOWN:
                keys = pygame.key.get_pressed()
                player.handleKeys(keys, resources)

        # Game logic goes here

        # Only for debug purpose
        player.vx, player.vy = 0, 0

        if random.random() < 0.01:  # 1% chance each frame
            resources.add(Resource(random.randint(0, WINDOW_WIDTH), random.randint(0, WINDOW_HEIGHT), 10))  # Spawn a high-value resource
        if random.random() < 0.1:  # 10% chance each frame
            resources.add(Resource(random.randint(0, WINDOW_WIDTH), random.randint(0, WINDOW_HEIGHT), 1))  # Spawn a low-value resource
        # end

        all_sprites.update()

        # Draw everything
        draw(window, all_sprites, resources)

    # Quit Pygame
    pygame.quit()

if __name__ == "__main__":
    main()

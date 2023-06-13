# import random
import pygame
from pygame.locals import *

from gameBoard import *
from players import *
from ressource import Resource 
from constantes import *

def draw(window, all_sprites, resources):
    # Draw everything
    all_sprites.draw(window)
    resources.draw(window)

    # Rafraîchissement de l'écran
    pygame.display.flip()

def main():
    pygame.init()

    # Ouverture de la fenêtre Pygame
    window = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT), RESIZABLE) 
    # #Icone
    # icone = pygame.image.load(image_icone)
    # pygame.display.set_icon(icone)
    # #Titre
    # pygame.display.set_caption(titre_fenetre)
    
    pygame.display.set_caption("Zappy")
    clock = pygame.time.Clock()

    # Chargement et collage du fond
    fond = pygame.image.load("spritesUsed/grassFlower2.jpeg").convert()
    fond = pygame.transform.scale(fond, (window.get_width(), window.get_height()))
    window.blit(fond, (0, 0))

    gameBoard = GameBoard(MAP_WIDTH, MAP_HEIGHT)
    # player = RabbitPlayer(gameBoard, 0, 0, 50, 50)
    # players = pygame.sprite.Group(player)
    # resources = pygame.sprite.Group()
    # addResources(resources)


    # Main game loop
    running = True
    while running:
        # Limit the frame rate
        # Ex: 30 frame by second
        clock.tick(FPS)

    # Event handling
        for event in pygame.event.get():
            if event.type == QUIT:
                running = False
            # elif event.type == COLLECT_RESOURCE_EVENT:
            #     collisions = pygame.sprite.spritecollide(player, resources, True)
            #     for collision in collisions:
            #         if collision.collecting:
            #             player.resources += collision.value
            #             collision.collecting = False
            #             collision.kill()
            # elif event.type == pygame.KEYDOWN:
            #     keys = pygame.key.get_pressed()
            #     player.handleKeys(keys, resources)

    # Game logic goes here

        # Only for debug purpose
        # player.vx, player.vy = 0, 0

        # if random.random() < 0.01:  # 1% chance each frame
        #     resources.add(Resource(random.randint(0, WINDOW_WIDTH), random.randint(0, WINDOW_HEIGHT), 10))  # Spawn a high-value resource
        # if random.random() < 0.1:  # 10% chance each frame
        #     resources.add(Resource(random.randint(0, WINDOW_WIDTH), random.randint(0, WINDOW_HEIGHT), 1))  # Spawn a low-value resource
        # end

        # players.update()
        # resources.update()

    # Draw everything
        fond = pygame.transform.scale(fond, (window.get_width(), window.get_height()))
        window.blit(fond, (0, 0))
        # draw(window, all_sprites, resources)
        gameBoard.render(window)
        pygame.display.update()

    # Quit Pygame
    pygame.quit()
    raise SystemExit

if __name__ == "__main__":
    main()

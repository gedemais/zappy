import pygame
from pygame.locals import *

from constantes import *

class Egg():
    def __init__(self, eggId, playerId, x, y):
        super().__init__()
        
        self.eggId = eggId
        self.parent = playerId
        self.position = (x, y)
        self.rect = self.image.get_rect()
        self.rect.x = x
        self.rect.y = y
        self.survival = 0
        self.connected = False

class Player():
    def __init__(self, gameBoard, playerId, team, x, y, o, level):
        super().__init__()

        # contain all info on players and map
        self.gameBoard = gameBoard

        # contain id of player and his team
        self.playerId = playerId
        self.team = team

        # Define player's base level
        self.level = level

        self.orientation = o
        # Define player's position
        self.position = (x, y)  # Start at the top-left corner of the game board
        
        # self.rect = self.image.get_rect()
        # self.rect.x = x
        # self.rect.y = y

        self.vx = 0 # horizontal movement length
        self.vy = 0 # vertical movement length
        self.speed = 5 # Adjust this value to change the player speed 

        # At the beginning the client has 10 life units, he can therefore survive 1260 time units, ie 1260/t seconds.
        # Initial survival time
        self.survival = 0

        # list of initial resources in backpack
        self.resources = {
            'nourriture': 10,
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }
        
    # One nourriture unit allows him to survive 126 time units, therefore 126/t seconds.
    def consumeNourriture(self):
        if self.resources['nourriture'] > 0:
            self.resources['nourriture'] -= 1
            self.survival += 125
        else:
            # The player is out of 'nourriture' and is removed from the game
            self.die()

    def updatePos(self, x, y, o):
        # Calculate new position
        # add animation
        #
        # new_x = (self.position[0] + self.vx) % self.gameBoard.width
        # new_y = (self.position[1] + self.vy) % self.gameBoard.height
        # self.position = (new_x, new_y)
        pass
    
    def updateLvl(self, level):
        self.level = level
    
    def updateInventory(self, resources):
        self.resources = resources

    def watch(self):
        # launch animation
        pass
    
    def update(self):
        # animate
        pass

    def collectResource(self):
        # launch animation to collect
        pass

    def throwResource(self, resourceId):
        # launch animation to throw
        pass
    
    def die(self):
        # launch death animation
        # For now, just print a message when a player dies
        print(f'Player {self.playerId} from team {self.team_id} has died.')

    def reproduce(self):
        # launch reproduce animation
        # For now, just print a message when a player reproduce
        print(f'Player {self.playerId} from team {self.team_id} laid an egg')

    def seeInventory(self):
        # launch animation
        #For now, just print the inventory in the console
        print(f'Player {self.playerId} from team {self.team_id} see his inventory : {str(self.resources)}')
    
    ############################################################################################### Only for debug purpose
        # Handle character movement and animation
        # if keys[K_LEFT] or keys[K_a]:
        #     self.animation_finished = False
        #     self.vx = -self.speed
        #     self.current_animation = 'walk_left'

        # if keys[K_RIGHT] or keys[K_d]:
        #     self.animation_finished = False
        #     self.vx = self.speed
        #     self.current_animation = 'walk_right'

        # if keys[K_UP] or keys[K_w]:
        #     self.animation_finished = False
        #     self.vy = -self.speed
        #     self.current_animation = 'walk_up'

        # if keys[K_DOWN] or keys[K_s]:
        #     self.animation_finished = False
        #     self.vy = self.speed
        #     self.current_animation = 'walk_down'

        # Handle collecting action
        # if keys[K_q] and pygame.sprite.spritecollide(self, resources, True):
        #     self.animation_finished = False
        #     self.current_animation = 'collect_' + self.current_animation.split('_')[1]

        #     # Check for collisions with resources for player
        #     collisions = pygame.sprite.spritecollide(self, resources, False)
        #     for collision in collisions:
        #         if not collision.collecting:
        #             collision.collecting = True
        #             pygame.time.set_timer(COLLECT_RESOURCE_EVENT, 2000)   # 2000 milliseconds = 2 seconds

        # Perform the ritual if possible
        # if keys[pygame.K_r]:
        #     # modify logic HERE
        #     if (self.resources > 0 and self.level == 0) or self.resources >= self.level * 10:
        #         self.animation_finished = False
        #         self.current_animation = 'levelup_'+ self.current_animation.split('_')[1]
        #         self.levelup()
        
    # end debug

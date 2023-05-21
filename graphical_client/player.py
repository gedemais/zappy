import pygame
from pygame.locals import *

from constantes import *

class Player():
    def __init__(self, game_board, player_id, team_id, x, y):
        super().__init__()

        # contain all info on players and map
        self.game_board = game_board

        # contain id of player and his team
        self.player_id = player_id
        self.team_id = team_id

        # Define player's base level
        self.level = 1

        # At the beginning the client has 10 life units, he can therefore survive 1260 time units, ie 1260/t seconds.
        # Initial survival time
        self.survival = 1260

        # Define player's position
        self.position = (x, y)  # Start at the top-left corner of the game board
        # self.rect = self.image.get_rect()
        # self.rect.x = x
        # self.rect.y = y

        self.vx = 0 # horizontal movement length
        self.vy = 0 # vertical movement length
        self.speed = 5 # Adjust this value to change the player speed 

        # list of initial resources in backpack
        self.resources = {
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0,
            'nourriture': 0
        }

    def move(self, dx, dy):
        # Calculate new position
        new_x = (self.position[0] + dx) % self.game_board.width
        new_y = (self.position[1] + dy) % self.game_board.height
        self.position = (new_x, new_y)

    def collect_resources(self):
        # Get the current cell
        cell = self.game_board.cells[self.position[1]][self.position[0]]
        
        # Add cell's resources to player's resources and clear the cell's resources
        for resource, amount in cell.resources.items():
            self.resources[resource] += amount
            cell.resources[resource] = 0

    def throw_resource(self, resource, amount):
        # Get the current cell
        cell = self.game_board.cells[self.position[1]][self.position[0]] 
        # Add player's resource to cell's resource and clear the player's resource
        cell.resources[resource] += amount
        self.resources[resource] -= amount

    # One nourriture unit allows him to survive 126 time units, therefore 126/t seconds.
    def consume_nourriture(self):
        if self.resources['nourriture'] > 0:
            self.resources['nourriture'] -= 1
            self.survival += 126
        else:
            # The player is out of 'nourriture' and is removed from the game
            self.die()

    def die(self):
        # For now, just print a message when a player dies
        print(f'Player {self.player_id} from team {self.team_id} has died.')

    def reproduce(self):
        # For now, just print a message when a player reproduce
        print(f'Player {self.player_id} from team {self.team_id} laid an egg')

    def seeInventory(self):
        #For now, just print the inventory int the console
        print(f'Player {self.player_id} from team {self.team_id} see his inventory : {str(self.resources)}')

    def level_up(self, recipes):

        recipe = recipes[self.level - 1]

        # First, check that the player has the necessary resources
        for resource, amount in recipe.items():
            if self.resources[resource] < amount:
                print(f"Not enough of resource {resource} to level up!")
                return False
            
        # Now, check the number and levels of other players in the same cell
        same_cell_players = [player for player in self.game_board.players if player.position == self.position and player.level == self.level]
        if len(same_cell_players) < self.level:
            return  # Not enough players in the same cell to level up

        # consume resources needed to level_up
        for resource, amount in recipe.items():
            self.resources[resource] -= amount

        # increase the level of all the participants
        for player_in_ritual in same_cell_players:
            player_in_ritual.level += 1
            print(f"Player {player_in_ritual.player_id} leveled up to level {player_in_ritual.level}!")

        # Check for win condition
        if self.level == len(recipes):
            print("Checking for win condition...")
            if self.check_win_condition():
                print(f"Team {self.team_id} has won the game!")
            else:
                print("Win condition not met. Game continues.")

    # Only for debug purpose
    def check_win_condition(self):
        # Get all players from the same team
        team_players = [player for player in self.game_board.players if player.team_id == self.team_id]

        # Check if all players in the team have reached the maximum level
        return all(player.level == self.level for player in team_players)

    def handleKeys(self, keys, resources):

        # Only for debug purpose
        # Handle character movement and animation
        if keys[K_LEFT] or keys[K_a]:
            self.animation_finished = False
            self.vx = -self.speed
            self.current_animation = 'walk_left'

        if keys[K_RIGHT] or keys[K_d]:
            self.animation_finished = False
            self.vx = self.speed
            self.current_animation = 'walk_right'

        if keys[K_UP] or keys[K_w]:
            self.animation_finished = False
            self.vy = -self.speed
            self.current_animation = 'walk_up'

        if keys[K_DOWN] or keys[K_s]:
            self.animation_finished = False
            self.vy = self.speed
            self.current_animation = 'walk_down'

        # Only for debug purpose
        # Handle collecting action
        if keys[K_q] and pygame.sprite.spritecollide(self, resources, True):
            self.animation_finished = False
            self.current_animation = 'collect_' + self.current_animation.split('_')[1]

            # Check for collisions with resources for player
            collisions = pygame.sprite.spritecollide(self, resources, False)
            for collision in collisions:
                if not collision.collecting:
                    collision.collecting = True
                    pygame.time.set_timer(COLLECT_RESOURCE_EVENT, 2000)   # 2000 milliseconds = 2 seconds

        # Only for debug purpose
        # Perform the ritual if possible
        if keys[pygame.K_r]:
            # modify logic HERE
            if (self.resources > 0 and self.level == 0) or self.resources >= self.level * 10:
                self.animation_finished = False
                self.current_animation = 'levelup_'+ self.current_animation.split('_')[1]
                self.levelup()

        # Add other event handlers here

        # Handle exit button
        if keys[K_ESCAPE]:
            pygame.event.post(QUIT)

    def update(self):
        self.animate()
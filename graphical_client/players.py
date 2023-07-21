from loaders import *
from player import Player

class RabbitPlayer(Player, RabbitPlayerLoader):
    def __init__(self, gameBoard, playerId, team_id, x, y):
        super().__init__(gameBoard, playerId, team_id, x, y) 
        self.race = 'rabbit'

# Add new type of player here (differents sprites), based on RabbitPlayer model.
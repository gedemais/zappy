from loaders import *
from player import Player

class RabbitPlayer(Player, RabbitPlayerLoader):
    def __init__(self, game_board, player_id, team_id, x, y):
        super().__init__(game_board, player_id, team_id, x, y)
        

# Add new type of player here (differents sprites), based on RabbitPlayer model.
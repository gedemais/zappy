from loaders import *
from player import Player

class RabbitPlayer(Player, RabbitPlayerLoader):
    def __init__(self):
        super().__init__()

# Add new type of player here (differents sprites), based on RabbitPlayer model.
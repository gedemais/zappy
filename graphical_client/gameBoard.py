import random

class Cell:
    def __init__(self, x , y):
        self.position = (x, y)
        self.players = []  # List of player ids present in this cell
        self.resources = {
            'nourriture': 0,
            'linemate': 0,
            'deraumere': 0,
            'sibur': 0,
            'mendiane': 0,
            'phiras': 0,
            'thystame': 0
        }

class GameBoard:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.cells = [[Cell(x, y) for x in range(width)] for y in range(height)]

    # Only for debug purpose
    def spawn_resources(self):
        for row in self.cells:
            for cell in row:
                 # Increase the chance of spawning resources
                if random.random() < 0.3:
                    cell.resources[random.choice(list(cell.resources.keys()))] += random.randint(1, 3)  # Add 1 to 3 resources

    def add_player(self, player):
        self.players.append(player)

    def applyCmd(self, cmdLine):

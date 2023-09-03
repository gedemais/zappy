from enum import Enum


class   S(Enum):
    """ States enumeration class  """
    IDLE = 0,
    WALKING_NORTH = 1,
    WALKING_EAST = 2,
    WALKING_SOUTH = 3,
    WALKING_WEST = 4,
    MAX = 5


class   Player():
    def __init__(self, x, y, o, lvl, team):
        self.x = x
        self.y = y
        self.o = o
        self.lvl = lvl
        self.inventory = []
        self.states_queue = []
        self.state = S.IDLE
        self.step = 0
        self.team = team
        self.eggs = {}

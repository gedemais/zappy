from transitions import Machine
from enum import Enum

class L(Enum):
	LOOT_FOOD = 0
	LOOT_LINEMATE = 1
	LOOT_DERAUMERE = 2
	LOOT_SIBUR = 3
	LOOT_MENDIANE = 4
	LOOT_PHIRAS = 5
	LOOT_THYSTAME = 6

class Brain:
    explored_tiles = []
    view = []
    loot_priorities = [5, 4, 3, 3, 2, 2, 4]
    x, y = 0, 0
    orientation = 0

    def __init__(self, ia):
        self.machine = Machine(model=self, states=["IDLE", "POPULATE", "LOOT"], initial="IDLE")
        self.machine.add_transition("go_loot", "IDLE", "LOOT")
        self.machine.add_transition("stop", "LOOT", "IDLE")
        self.ia = ia

    def update(self):
        if self.state == LOOT:
            self.loot()
        elif self.state == IDLE:
            continue

    def populate(self):

    def loot(self):
        #if len(view) == 0 or self.view_size == len(view):
        #   Randomize orientation
        #   Launch view command and parse result in an array of arrays
        #   self.view_size = len(view)
        #
        #if (x, y) in explored_tiles:
        #    return
        #
        # if len(view[0] > 0):
        #   Ramasser les ressources interessantes sur la case en fonction des prios
        #   explored_tiles.append((x, y))
        #   Progresser dans la view

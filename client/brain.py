from transitions import Machine
from enum import Enum
from command import C, Command

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
    blind = True

    def __init__(self):
        self.machine = Machine(model=self, states=["IDLE", "POPULATE", "LOOT"], initial="IDLE")
        self.machine.add_transition("go_loot", "IDLE", "LOOT")
        self.machine.add_transition("stop", "LOOT", "IDLE")

    def update(self):
        if self.state == "LOOT":
            return self.loot()
        elif self.state == "IDLE":
            return

    def populate(self):
        print("toto")

    def loot(self):
         if (self.blind == True):
              return (Command(id = C.VOIR))
        #  return [
        #       Command(id = C.VOIR),
        #       Command(id = C.AVANCE),
        #       Command(id = C.AVANCE),
        #       Command(id = C.AVANCE),
        #       Command(id = C.DROITE),
        #       Command(id = C.PREND, buf = "nourriture"),
		#  ]
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

from transitions import Machine
from enum import Enum
from command import C, Command
from random import randint

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
        self.machine.add_transition("fetch", "IDLE", "LOOT")
        self.machine.add_transition("stop", "LOOT", "IDLE")

    def update_state(self, client):
        pass

    def update(self, client):
        if self.state == "LOOT":
            return self.loot(client)
        elif self.state == "IDLE":
            return

    def populate(self):
        print("toto")

    def loot(self, client):
        commands = []
        viewed = False

        if viewed == False and (len(self.view) == 0 or self.view_size == len(self.explored_tiles)):
            commands.append(C.DROITE if randint(0, 100) < 50 else C.GAUCHE)
            commands.append(C.VOIR)
            viewed = True

        if viewed and len(client.qreceive.buf) > 0:
            response = client.qreceive.buf

            if response[-1][0] == '{' and response[-1][-1] == '}':
                viewed = False
                response = client.qreceive.buf[-1][1:-1]
                print(response)
                for tile in response.split(','):
                    self.view.append([loot for loot in tile.split(' ')].remove(''))
                self.view_size = len(self.view)
                print(self.view)
                print(self.view_size)
        #
        #if (x, y) in explored_tiles:
        #    return
        #
        # if len(view[0] > 0):
        #   Ramasser les ressources interessantes sur la case en fonction des prios
        #   explored_tiles.append((x, y))
        #   Progresser dans la view

        for i, c in enumerate(commands):
            commands[i] = Command(id = c)
        return commands

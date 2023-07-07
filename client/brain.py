from transitions import Machine
from enum import Enum
from command import C, S, Command
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
    # Explorated tiles
    explored_tiles = []
    view = []
    loot_priorities = [5, 4, 3, 3, 2, 2, 4]
    # Position in the bot's referential
    x, y = 0, 0
    # Tile index in view's tiles referential (amount depending on bot level)
    tile = 0
    # Has the player ever seen ?
    blind = True

    def __init__(self):
        self.machine = Machine(model=self, states=["IDLE", "POPULATE", "LOOT"], initial="IDLE")
        self.machine.add_transition("fetch", "IDLE", "LOOT")
        self.machine.add_transition("stop", "LOOT", "IDLE")
        self.busy = True

    def update(self, client, cmd):
        if self.state == "LOOT":
            return self.loot(client)
        elif self.state == "IDLE":
            return

    def populate(self):
        print("toto")

    def loot(self, client):
        commands = []

        if self.blind and len(self.view) == 0:
            commands.append(C.DROITE if randint(0, 100) < 50 else C.GAUCHE)
            commands.append(C.VOIR)
            self.blind = False

        if not self.blind and len(client.qreceive.buf) > 0:
            response = client.qreceive.buf

            if response[-1][0] == '{' and response[-1][-1] == '}' and 'ttl' not in response:
                self.blind = False
                response = client.qreceive.buf[-1][1:-1]
                for tile in response.split(','):
                    loot = tile.split(' ')
                    while '' in loot:
                        loot.remove('')
                    self.view.append(loot)

                self.view_size = len(self.view)
        
        if (self.x, self.y) in self.explored_tiles:
            # Move it on the next tile of its view
            pass

        #if len(view[self.tile]) > 0: # and not busy
        #    if 
        #    commands.append(id=C.TAKE, buf='')

        #    pass
        #   Ramasser les ressources interessantes sur la case en fonction des prios
        #   explored_tiles.append((self.x, self.y))
        #   Progresser dans la view

        for i, c in enumerate(commands):
            print(c)
            commands[i] = Command(id = c)
        return commands if self.busy == False else []

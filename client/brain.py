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
		self.busy = True

	def update(self, client, cmd):
		if self.state == "LOOT":
			return self.loot(client, cmd)
		elif self.state == "IDLE":
			return

	def loot(self, client, cmd):
		commands = []

		if self.blind == True:
			commands.append(C.VOIR)

		if cmd.response[-1][0] == '{' and cmd.response[-1][-1] == '}':
			response = client.qreceive.buf[-1][1:-1]
			for tile in response.split(','):
				self.view.append([loot for loot in tile.split(' ')].remove(''))
			self.view_size = len(self.view)

		for i, c in enumerate(commands):
			commands[i] = Command(id = c)
		return commands if self.busy == False else []

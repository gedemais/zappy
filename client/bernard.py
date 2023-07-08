from transitions import Machine
from enum import Enum
from command import C, S, Command
from random import randint
from brain import Brain


class	L(Enum):
	FOOD = 0
	LINEMATE = 1
	DERAUMERE = 2
	SIBUR = 3
	MENDIANE = 4
	PHIRAS = 5
	THYSTAME = 6

class	IA:
	# Explorated tiles
	explored_tiles = []
	view = []
	view_size = 0
	loot_priorities = [5, 4, 3, 3, 2, 2, 4]
	# Position in the bot's referential
	x, y = 0, 0
	# Tile index in view's tiles referential (amount depending on bot level)
	tile = 0
	# bernard command
	cmd = Command()
	inventory = []
	needs = {
			C.CONNECT_NBR	: 0,
			C.INVENTAIRE	: 1,
			C.VOIR			: 1,
			C.AVANCE		: 0,
			C.DROITE		: 0,
			C.GAUCHE		: 0,
			C.PREND			: 1,
			C.POSE			: 0,
			C.INCANTATION	: 0,
			C.FORK			: 0,
			C.EXPULSE		: 0,
			C.BROADCAST		: 0,
		}

	def __init__(self):
		self.brain = Brain()
		self.machine = Machine(model=self, states=["IDLE", "RUSH"], initial="IDLE")
		self.machine.add_transition("rush", "IDLE", "RUSH")
		self.machine.add_transition("stop", "RUSH", "IDLE")

	def	interact(self):
		if self.brain.cmd.state == S.RECEIVED:
			self.cmd = self.brain.cmd.copy()
		if self.brain.busy == False:
			commands = self.rush()
			self.brain.input(commands)
		self.brain.process()

	# def update(self):
	# 	if self.state == "RUSH":
	# 		self.rush()
	# 	elif self.state == "IDLE":
	# 		pass

	def	await_response(self, id):
		if self.cmd.id == id and self.cmd.state == S.RECEIVED:
			return True
		return False
	
	def	execute(self, commands, id, buf = None):
		if self.await_response(id) == True:
			return True
		if self.needs[id] == 1:
			command = Command(id = id)
			if buf != None:
				command.buf = buf
			commands.append(command)
			return False
		return True
	
	def	callback(self, id):
		if self.needs[id] == 1 and self.cmd.state == S.RECEIVED:
			self.needs[id] = 0
			return True
		return False

	def rush(self):

		print("road to level 2 !", self.cmd.debug())
		commands = []
		item = "nourriture"

		if self.execute(commands, C.VOIR) == False:
			return commands
		elif self.callback(C.VOIR) == True:
			for loot in self.cmd.response:
				self.view.append(loot)
			self.view_size = len(self.view)

		if self.execute(commands, C.INVENTAIRE) == False:
			return commands
		elif self.callback(C.INVENTAIRE) == True:
			self.inventory = self.cmd.response

		if self.execute(commands, C.PREND, item) == False:
			return commands
		elif self.callback(C.PREND) == True:
			self.inventory[item] = self.inventory[item] + 1
			self.needs[C.AVANCE] = 1

		if self.execute(commands, C.AVANCE) == False:
			return commands
		elif self.callback(C.AVANCE) == True:
			self.needs[C.PREND] = 1
		
		return commands

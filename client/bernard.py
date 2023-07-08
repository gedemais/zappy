from transitions import Machine
from enum import Enum
from command import C, S, Command
from random import randint
from brain import Brain


class	L(Enum):
	LOOT_FOOD = 0
	LOOT_LINEMATE = 1
	LOOT_DERAUMERE = 2
	LOOT_SIBUR = 3
	LOOT_MENDIANE = 4
	LOOT_PHIRAS = 5
	LOOT_THYSTAME = 6

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
	# Has the player ever seen ?
	blind = True
	# bernard command
	cmd = Command()
	# commands to send to bernard when he's not busy
	commands = []
	# inventaire
	inventory = []

	def __init__(self):
		self.brain = Brain()
		self.machine = Machine(model=self, states=["IDLE", "LOOT"], initial="IDLE")
		self.machine.add_transition("fetch", "IDLE", "LOOT")
		self.machine.add_transition("stop", "LOOT", "IDLE")

	def	interact(self):
		if self.brain.cmd.state == S.RECEIVED:
			self.cmd.reset(id = self.brain.cmd.id, response = self.brain.cmd.response, state = self.brain.cmd.state)
		if self.brain.busy == False:
			self.update()
			self.brain.input(self.commands)
		self.brain.process()

	def update(self):
		self.commands = []
		if self.state == "LOOT":
			self.loot()
		elif self.state == "IDLE":
			pass

	def	vision(self):
		if self.cmd.id == C.VOIR and self.cmd.state == S.RECEIVED:
			for loot in self.cmd.response:
				self.view.append(loot)
			self.view_size = len(self.view)
			if (self.view_size > 0):
				self.blind = False
			#on reset la cmd car elle vient d'être traitée
			self.cmd.reset()
		if self.blind:
			self.commands.append(Command(id = C.VOIR))
			return False
		return True
	
	def	inventaire(self):
		if self.cmd.id == C.INVENTAIRE and self.cmd.state == S.RECEIVED:
			self.inventory = self.cmd.response
			#on reset la cmd car elle vient d'être traitée
			self.cmd.reset()

		if self.cmd.id == None:
			self.commands.append(Command(id = C.INVENTAIRE))
			return False

		return True

	def loot(self):

		print("loot")

		if self.vision() == False:
			return
		
		if self.inventaire() == False:
			return

		self.commands += [
			Command(id = C.AVANCE), Command(id = C.PREND, buf="nourriture"),
			Command(id = C.AVANCE), Command(id = C.PREND, buf="nourriture"),
			Command(id = C.AVANCE), Command(id = C.PREND, buf="nourriture"),
			Command(id = C.AVANCE), Command(id = C.PREND, buf="nourriture"),
			Command(id = C.DROITE),
		]

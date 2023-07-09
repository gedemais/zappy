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

class	T(Enum):
	NONE = 0
	MANGER = 1
	INCANT_GATHER = 2
	INCANT_PUT = 3
	INCANT = 4

class	IA:
	# Explorated tiles
	explored_tiles = []
	# Tile index in view's tiles referential (amount depending on bot level)
	tile = 0
	# view data
	view = []
	view_size = 0
	# Position and Direction in bot's referential
	x, y = 0, 0
	#position from start
	sx, sy = 0, 0
	dir = 0
	#server speed and ticks ellapsed
	ticks, t, te = 0, 0, 0
	# bernard command
	cmd = Command()
	lvl = 1
	inventory = []
	needs = {
		#Tasks
		T.MANGER		: Command(),
		T.INCANT_GATHER	: Command(),
		T.INCANT_PUT	: Command(),
		T.INCANT		: Command(),
		#Commands
		C.CONNECT_NBR	: Command(),
		C.INVENTAIRE	: Command(),
		C.VOIR			: Command(),
		C.AVANCE		: Command(),
		C.DROITE		: Command(),
		C.GAUCHE		: Command(),
		C.PREND			: Command(),
		C.POSE			: Command(),
		C.INCANTATION	: Command(),
		C.FORK			: Command(),
		C.EXPULSE		: Command(),
		C.BROADCAST		: Command(),
	}

	def __init__(self):
		self.brain = Brain()
		self.machine = Machine(model=self, states=["IDLE", "RUSH"], initial="IDLE")
		self.machine.add_transition("fetch", "IDLE", "RUSH")
		self.machine.add_transition("stop", "RUSH", "IDLE")

	def update(self):
		if self.state == "RUSH":
			return self.rush()
		elif self.state == "IDLE":
			pass

	def	update_bot(self, id, item = None):
		if id == C.VOIR:
			self.view = []
			self.x, self.y = 0, 0
			for loot in self.cmd.response:
				self.view.append(loot)
			self.view_size = len(self.view)
		elif id == C.INVENTAIRE:
			self.inventory = self.cmd.response
			self.needs[C.INVENTAIRE].buf = self.ticks
		elif id == C.PREND:
			for element in self.inventory:
				if element == self.cmd.buf:
					self.inventory[element] = self.inventory[element] + 1
			viewcase = self.view[self.getviewindex()]
			if self.cmd.buf in viewcase:
				viewcase[self.cmd.buf] = viewcase[self.cmd.buf] - 1
		elif id == C.POSE:
			for element in self.inventory:
				if element == self.cmd.buf:
					self.inventory[element] = self.inventory[element] - 1
			viewcase = self.view[self.getviewindex()]
			if self.cmd.buf in viewcase:
				viewcase[self.cmd.buf] = viewcase[self.cmd.buf] + 1
		elif id == C.DROITE:
			self.dir = self.dir + 90
			if self.dir == 270:
				self.dir = -90
		elif id == C.GAUCHE:
			self.dir = self.dir - 90
			if self.dir == -270:
				self.dir = 90
		elif id == C.AVANCE:
			#front
			if self.dir == 0:
				self.x = self.x + 1
				self.sx = self.sx + 1
			#back
			if self.dir == 180 or self.dir == -180:
				self.x = self.x - 1
				self.sx = self.sx - 1
			#right
			if self.dir == 90:
				self.y = self.y + 1
				self.sy = self.sy + 1
			#left
			if self.dir == -90:
				self.y = self.y - 1
				self.sy = self.sy - 1

	def	interact(self, ticks):
		self.ticks = ticks
		if self.brain.cmd.state == S.RECEIVED:
			self.cmd = self.brain.cmd.copy()
		if self.brain.busy == False:
			commands = self.update()
			self.brain.input(commands)
			self.cmd.reset()
		self.brain.process()

	def	await_response(self, id):
		if self.cmd.id == id and self.cmd.state == S.RECEIVED:
			return True
		return False

	def	callback(self, id):
		if self.needs[id].state == S.PENDING and self.cmd.state == S.RECEIVED:
			self.update_bot(id)
			self.needs[id].state = S.NONE
			return True
		return False
	
	def	execute(self, commands, id):
		if self.await_response(id) == True:
			return True
		if self.needs[id].state == S.PENDING:
			command = Command(id = id)
			print("exec:", command.id)
			buf = self.needs[id].buf
			if buf != None:
				if type(buf) == list:
					for elt in buf:
						command = Command(id = id, buf = elt)
						commands.append(command)
				else:
					command = Command(id = id, buf = buf)
			commands.append(command)
			return False
		return True

	#WIP
	def	getviewindex(self):
		index = 0
		middle = self.x * (self.x + 1)
		if self.y < 0:
			index = middle - self.y
		else:
			index = middle + self.y
		return index
	#WIP
	def	outofview(self):
		range = self.lvl
		if self.x < 0 or self.x > range:
			return True
		range = abs(self.lvl - self.lvl - self.x)
		if self.y < -range or self.y > range:
			return True
		return False
	#WIP
	def	task_manager(self):
		if self.needs[T.MANGER].state == S.PENDING:
			#il faut trouver de la nourriture
			print("T.MANGER")
			#suis-je en dehors de mon champ de vision ?
			if self.outofview() == True:
				self.needs[C.VOIR].state = S.PENDING
				return
			#y a t-il de la nourriture ou je suis ?
			#oui : prendre
			index = self.getviewindex()
			print(self.view[index])
			if "nourriture" in self.view[index] and self.view[index]["nourriture"] > 0:
				print("food on bot pos, on prend")
				self.needs[C.PREND].reset(state = S.PENDING, buf = "nourriture")
			#non : avancer
			else:
				print("no food on bot pos, on avance")
				self.needs[C.AVANCE].state = S.PENDING		

	def	manager(self):
		if len(self.view) == 0:
			self.needs[C.VOIR].state = S.PENDING
			return
		if len(self.inventory) == 0:
			self.needs[C.INVENTAIRE].reset(state = S.PENDING)
		else:
			self.needs[T.MANGER].state = S.PENDING if self.inventory["nourriture"] < 10 else S.NONE

		#WIP
		if self.needs[C.INVENTAIRE].buf != None:
			print("inventaire will update {} / {}".format(self.ticks - self.needs[C.INVENTAIRE].buf, self.te * 5))
		if self.needs[C.INVENTAIRE].buf != None and self.ticks - self.needs[C.INVENTAIRE].buf > self.te * 5:
			self.needs[C.INVENTAIRE].reset(state = S.PENDING)

	def rush(self):
		commands = []
		self.manager()

		if self.execute(commands, C.VOIR) == False:
			self.t = self.ticks
			return commands
		elif self.callback(C.VOIR) == True:
			self.te = self.ticks - self.t
			print("callback voir")

		if self.execute(commands, C.INVENTAIRE) == False:
			return commands
		elif self.callback(C.INVENTAIRE) == True:
			print("callback inventaire")
			print(self.inventory)

		if self.execute(commands, C.AVANCE) == False:
			return commands
		elif self.callback(C.AVANCE) == True:
			print("callback avance", "x", self.x, "y", self.y, "viewindex", self.getviewindex())

		if self.execute(commands, C.PREND) == False:
			return commands
		elif self.callback(C.PREND) == True:
			print("callback prend", self.needs[C.PREND].buf)

		self.task_manager()
		return commands

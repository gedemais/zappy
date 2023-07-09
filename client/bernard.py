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
	INCANTATION = 2

class	IA:
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
	tasks = {
		T.MANGER		: Command(id = T.MANGER),
		T.INCANTATION	: Command(id = T.INCANTATION),
	}
	needs = {
		C.CONNECT_NBR	: Command(id = C.CONNECT_NBR),
		C.INVENTAIRE	: Command(id = C.INVENTAIRE),
		C.VOIR			: Command(id = C.VOIR),
		C.AVANCE		: Command(id = C.AVANCE),
		C.DROITE		: Command(id = C.DROITE),
		C.GAUCHE		: Command(id = C.GAUCHE),
		C.PREND			: Command(id = C.PREND),
		C.POSE			: Command(id = C.POSE),
		C.INCANTATION	: Command(id = C.INCANTATION),
		C.FORK			: Command(id = C.FORK),
		C.EXPULSE		: Command(id = C.EXPULSE),
		C.BROADCAST		: Command(id = C.BROADCAST),
	}

	def __init__(self, wx, wy):
		self.brain = Brain()
		self.machine = Machine(model=self, states=["IDLE", "RUSH"], initial="IDLE")
		self.machine.add_transition("fetch", "IDLE", "RUSH")
		self.machine.add_transition("stop", "RUSH", "IDLE")
		#world size
		self.wx, self.wy = wx, wy

	def	interact(self, ticks):
		self.ticks = ticks
		#si le brain n'est pas occuppé on prépare une nouvelle suite de commands à transceive
		if self.brain.busy == False:
			commands = self.update(ticks)
			self.brain.input(commands)
		#on process le brain
		self.brain.process()

	def update(self):
		if self.state == "RUSH":
			return self.rush()
		elif self.state == "IDLE":
			pass

	#comportement par défaut des fonctions lors des callbacks
	def	update_bot(self, command):
		#on stock view
		if command.id == C.VOIR:
			#WIP
			if self.te == 0:
				self.te = self.ticks - self.t
			#
			self.view = []
			#on reset la position du joueur
			self.x, self.y = 0, 0
			for loot in command.response:
				self.view.append(loot)
			self.view_size = len(self.view)
		#on stock l'inventaire
		elif command.id == C.INVENTAIRE:
			self.inventory = command.response
			#on stock le tick du dernier update
			self.needs[C.INVENTAIRE].buf = self.ticks
		#on prend les elements sur la case et on update l'inventaire et la viewcase
		elif command.id == C.PREND:
			for element in self.inventory:
				if element == command.buf:
					self.inventory[element] = self.inventory[element] + 1
			viewcase = self.view[self.getviewindex()]
			if command.buf in viewcase:
				viewcase[command.buf] = viewcase[command.buf] - 1
		#on pose les elements sur la case et on update l'inventaire et la viewcase
		elif command.id == C.POSE:
			for element in self.inventory:
				if element == command.buf:
					self.inventory[element] = self.inventory[element] - 1
			viewcase = self.view[self.getviewindex()]
			if command.buf in viewcase:
				viewcase[command.buf] = viewcase[command.buf] + 1
		#right rotation
		elif command.id == C.DROITE:
			self.dir = self.dir + 90
			if self.dir == 270:
				self.dir = -90
			print("dir", self.dir)
		#left rotation
		elif command.id == C.GAUCHE:
			self.dir = self.dir - 90
			if self.dir == -270:
				self.dir = 90
			print("dir", self.dir)
		#movement forward
		elif command.id == C.AVANCE:
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
			print("pos x {} y {} spos {} {}".format(self.x, self.y, self.sx, self.sy))

	#fonction à executer quand le state est pending et qu'on a reçu une response
	def	receive(self, id):
		for command in self.brain.memory:
			if command.id == id:
				print("receive", id)
				self.update_bot(command)
				self.needs[id].state = S.NONE
	
	def	transceive(self, commands, id):
		print("transceive", id)
		command = Command(id = id)
		buf = self.needs[id].buf
		if buf != None:
			if type(buf) == list:
				for elt in buf:
					command = Command(id = id, buf = elt)
					commands.append(command)
			else:
				command = Command(id = id, buf = buf)
		commands.append(command)

	#on return True si on a une reponse a la cmd envoyée
	def	await_response(self, id):
		for command in self.brain.memory:
			if command.id == id and command.state == S.RECEIVED:
				return True
		return False
	
	def	callback(self):
		#si un need est pending et que cmd.id == need.id et cmd.state == received
		for i in self.needs:
			command = self.needs[i]
			if self.await_response(command.id) == True and command.state == S.NEEDED:
				self.receive(command.id)

	def	call(self, commands):
		#si un need est pending et que l'on a pas de reponse
		for i in self.needs:
			command = self.needs[i]
			if self.await_response(command.id) == False and command.state == S.NEEDED:
				self.transceive(commands, command.id)

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
		if self.view == None or len(self.view) == 0:
			return True
		range = self.lvl
		if self.x < 0 or self.x > range:
			return True
		range = abs(self.lvl - self.lvl - self.x)
		if self.y < -range or self.y > range:
			return True
		return False

	#WIP
	def	task_assign(self):
		#on reset les tasks en cours
		for task in self.tasks:
			self.tasks[task].state = S.NONE
		if self.inventory != None and len(self.inventory) > 0 and self.inventory["nourriture"] < 10:
			self.tasks[T.MANGER].state = S.NEEDED
			return
		#si le bot ne meurs pas de fin on va tenter une incantation
		self.tasks[T.INCANTATION].state = S.NEEDED

	#assigne une tache au joueur celon ses besoins
	def	task_manager(self):
		print("start task manager -----")
		self.task_assign()
		if self.tasks[T.MANGER].state == S.NEEDED:
			#il faut trouver de la nourriture
			print("T.MANGER")
			#suis-je en dehors de mon champ de vision ?
			if self.outofview() == True:
				self.needs[C.VOIR].state = S.NEEDED
				self.needs[C.DROITE if randint(0, 100) < 50 else C.GAUCHE].state = S.NEEDED
				return
			#y a t-il de la nourriture ou je suis ?
			#oui : prendre
			index = self.getviewindex()
			print(self.view[index])
			if "nourriture" in self.view[index] and self.view[index]["nourriture"] > 0:
				print("food on bot pos, on prend")
				self.needs[C.PREND].reset(id = C.PREND, state = S.NEEDED, buf = "nourriture")
			#non : avancer
			else:
				print("no food on bot pos, on avance")
				self.needs[C.AVANCE].state = S.NEEDED	
		if self.tasks[T.INCANTATION].state == S.NEEDED:
			print("T.INCANTATION")
		print("end task manager -------")

	#celon les données de bernard on assigne de nouvelles taches
	def	manager(self):
		print("start manager ----------")
		#first view
		if self.view == None or len(self.view) == 0:
			self.t = self.ticks
			self.needs[C.VOIR].state = S.NEEDED
		#first inventory
		if self.inventory == None or len(self.inventory) == 0:
			self.needs[C.INVENTAIRE].state = S.NEEDED
		#WIP
		if self.needs[C.INVENTAIRE].buf != None:
			print("inventaire update", self.te * 4 - (self.ticks - self.needs[C.INVENTAIRE].buf))
			if self.ticks - self.needs[C.INVENTAIRE].buf > self.te * 4:
				self.needs[C.INVENTAIRE].reset(id = C.INVENTAIRE, state = S.NEEDED)
		print("end manager ------------")
		#WIP
		self.task_manager()

	def rush(self):
		print("rush - start loop ================")
		commands = []

		self.callback()
		self.manager()
		self.call(commands)

		print("rush - end loop ==================")
		return commands

from transitions import Machine
from enum import Enum

from command import C, S, Command
from brain import Brain
from action import Action
from rush import Rush


class	L(Enum):
	FOOD = 0
	LINEMATE = 1
	DERAUMERE = 2
	SIBUR = 3
	MENDIANE = 4
	PHIRAS = 5
	THYSTAME = 6

class	IA:
	#time
	t = 0
	# view data
	view = []
	view_size = 0
	# position from start
	sx, sy = 0, 0
	# Position and Direction in bot's referential
	x, y = 0, 0
	dir = 0
	# bernard
	lvl = 1
	inventory = []
	update_inventory = 0
	needs = {
		C.CONNECT_NBR	: Command(id = C.CONNECT_NBR, callback = Action.connect_nbr ),
		C.INVENTAIRE	: Command(id = C.INVENTAIRE, callback = Action.inventaire ),
		C.VOIR			: Command(id = C.VOIR, callback = Action.voir ),
		C.AVANCE		: Command(id = C.AVANCE, callback = Action.avance ),
		C.DROITE		: Command(id = C.DROITE, callback = Action.droite ),
		C.GAUCHE		: Command(id = C.GAUCHE, callback = Action.gauche ),
		C.PREND			: Command(id = C.PREND, callback = Action.prend ),
		C.POSE			: Command(id = C.POSE, callback = Action.pose ),
		C.INCANTATION	: Command(id = C.INCANTATION, callback = Action.incantation ),
		C.FORK			: Command(id = C.FORK, callback = Action.fork ),
		C.EXPULSE		: Command(id = C.EXPULSE, callback = Action.expulse ),
		C.BROADCAST		: Command(id = C.BROADCAST, callback = Action.broadcast ),
	}
	actions = []

	def __init__(self, wx, wy):
		self.brain = Brain()
		self.rush = Rush()
		self.machine = Machine(model=self, states=["IDLE", "RUSH"], initial="IDLE")
		self.machine.add_transition("fetch", "IDLE", "RUSH")
		self.machine.add_transition("stop", "RUSH", "IDLE")
		#world size
		self.wx, self.wy = wx, wy

	def	interact(self, t):
		self.t = t
		#si le brain n'est pas occuppé on prépare une nouvelle suite de commands à transceive
		if self.brain.busy == False:
			commands = self.update()
			self.brain.input(commands)
		#on process le brain
		self.brain.process()

	def update(self):
		commands = []

		self.callback()
		if self.state == "RUSH":
			self.rush.run(self)
		elif self.state == "IDLE":
			pass
		self.call(commands)
		return commands

	#fonction à executer quand le state est pending et qu'on a reçu une response
	def	receive(self, id):
		for command in self.brain.memory:
			if command.id == id:
				if "ko" not in command.response:
					print("receive", id)
					self.needs[command.id].callback(self, command)
				else:
					print("response is ko")
				self.needs[id].update(state = S.NONE)
	
	#fonction pour append la command
	def	transceive(self, commands, cmd):
		print("transceive", cmd.id)
		for i in range(cmd.repeat):
			if cmd.buf != None:
				if type(cmd.buf) == list:
					for elt in cmd.buf:
						commands.append(Command(id = cmd.id, buf = elt))
				else:
					commands.append(Command(id = cmd.id, buf = cmd.buf))
			else:
				commands.append(Command(id = cmd.id))

	#on return True si on a une reponse a la cmd envoyée
	def	await_response(self, id):
		for command in self.brain.memory:
			if command.id == id and command.state == S.RECEIVED:
				return True
		return False
	
	def	callback(self):
		#si un need est pending et que la cmd est received (dans brain.memory)
		for i in self.needs:
			command = self.needs[i]
			if self.await_response(command.id) == True and command.state == S.NEEDED:
				self.receive(command.id)

	def	call(self, commands):
		#si la queue d'actions n'est pas vide et que l'on a pas de reponse
		for command in self.actions:
			if self.await_response(self.needs[command.id].id) == False and self.needs[command.id].state == S.NEEDED:
				self.transceive(commands, command)
		self.actions = []

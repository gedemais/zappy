from utils.command import C, S, Command
from utils.brain import Brain
from action.callback import Callback
from action.message import Message
from config.maboye import Maboye


class	IA:
	id = None
	t = 0
	bdir = []
	view = []
	view_size = 0
	x, y = 0, 0
	lvl = 1
	rushlvl = 2
	rushfinal = False
	hatched = False
	foodmin = 5
	foodmax = 10
	leader = None
	leader_contact = 0
	team_slot = 0
	team_total = 0
	inventory = []
	last_broadcast = 0
	last_inventory = 0
	last_hatch = 0
	suicide = False
	#permet de savoir quand un call a été effectué
	needs = {
		C.CONNECT_NBR	: Command(id = C.CONNECT_NBR, callback = Callback.connect_nbr ),
		C.INVENTAIRE	: Command(id = C.INVENTAIRE, callback = Callback.inventaire ),
		C.VOIR			: Command(id = C.VOIR, callback = Callback.voir ),
		C.AVANCE		: Command(id = C.AVANCE, callback = Callback.avance ),
		C.DROITE		: Command(id = C.DROITE, callback = Callback.droite ),
		C.GAUCHE		: Command(id = C.GAUCHE, callback = Callback.gauche ),
		C.PREND			: Command(id = C.PREND, callback = Callback.prend ),
		C.POSE			: Command(id = C.POSE, callback = Callback.pose ),
		C.INCANTATION	: Command(id = C.INCANTATION, callback = Callback.incantation ),
		C.FORK			: Command(id = C.FORK, callback = Callback.fork ),
		C.EXPULSE		: Command(id = C.EXPULSE, callback = Callback.expulse ),
		C.BROADCAST		: Command(id = C.BROADCAST, callback = Callback.broadcast ),
	}
	#queue des actions qui vont être call
	actions = []

	def __init__(self, host, port, team_name, wx, wy):
		self.name = "bernard"
		self.brain = Brain()
		self.host = host
		self.port = port
		self.team_name = team_name
		#world size
		self.wx, self.wy = wx, wy
		self.wr = (self.wx + self.wy) / 2

	def	interact(self, t, server_messages):
		self.t = t

		self.leader_order = None
		#on regarde si on a reçu des messages de la part du serveur
		if len(server_messages) > 0:
			self.handle_server_messages(server_messages)
		#si le brain n'est pas occuppé on prépare une nouvelle suite de commands à transceive
		if self.brain.busy == False:
			commands = self.update()
			self.brain.input(commands)
		#on process le brain
		#le brain va return False until les commands input ont toutes été traitées par le serveur
		self.brain.process()

	def update(self):
		commands = []

		self.callback()
		Maboye.run(self)
		self.call(commands)
		return commands

	def	check_team_id(self, server_message):
		split = server_message.split(',')
		message = split[1].strip()
		if "team_name " in message:
			split = message.split(' ')
			if self.team_name == split[1]:
				return True
		return False

	def	handle_server_messages(self, server_messages):
		if len(server_messages) > 0:
			print("MESSAGE RECEIVED ---------")
			for message in server_messages:
				if "You have to wait for the game to start" in message:
					#game didn't start yet
					Message.start(self, message)
				elif "message" in message:
					#server send a broadcast
					print(message)
					if self.check_team_id(message) == True:
						Message.message(self, message)
				elif "deplacement" in message:
					#server send a kick
					Message.kick(self, message)
			print("--------------------------")

	#fonction à executer quand le state est pending et qu'on a reçu une response
	def	receive(self, cmd):
		for command in self.brain.memory:
			if command.state == S.PENDING and command.id == cmd.id:
				if "ko" not in command.response:
					self.needs[command.id].callback(self, command)
				else:
					print("{} {} : KO".format(command.id, command.buf))
					self.ko = True
				command.state = S.NONE
	
	#fonction pour append la command
	def	transceive(self, commands, cmd):
		if cmd.buf != None:
			if type(cmd.buf) == list:
				for elt in cmd.buf:
					commands.append(Command(id = cmd.id, buf = elt))
			else:
				commands.append(Command(id = cmd.id, buf = cmd.buf))
		else:
			commands.append(Command(id = cmd.id))

	#on return True si on a une reponse a la cmd envoyée
	def	await_response(self, cmd):
		for command in self.brain.memory:
			if command.state == S.RECEIVED and command.id == cmd.id \
					and cmd.buf == command.buf:
				command.state = S.PENDING
				return True
		return False
	
	def	callback(self):
		self.ko = False
		#si un need est pending et que la cmd est received (dans brain.memory)
		for command in self.actions:
			if self.await_response(command) == True:
				self.receive(command)
		self.actions = []

	def	call(self, commands):
		#si la queue d'actions n'est pas vide et que l'on a un need
		for command in self.actions:
			self.transceive(commands, command)

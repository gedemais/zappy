from callback import Callback
from command import C, S, Command


class	Bot:
	def __init__(self, client_data):
		self.alive = True
		self.qreceive = client_data.qreceive
		self.qtransceive = client_data.qtransceive
		self.cmd = Command(id = C.CONNECT_NBR)
		self.cmds = {
			C.CONNECT_NBR	: Command(command = "connect_nbr", callback = Callback.connect_nbr),
			C.INVENTAIRE	: Command(command = "inventaire", callback = Callback.inventaire),
			C.VOIR			: Command(command = "voir", callback = Callback.voir),
			C.AVANCE		: Command(command = "avance", callback = Callback.avance),
			C.DROITE		: Command(command = "droite", callback = Callback.droite),
			C.GAUCHE		: Command(command = "gauche", callback = Callback.gauche),
			C.PREND			: Command(command = "prend", callback = Callback.prend),
			C.POSE			: Command(command = "pose", callback = Callback.pose),
			C.INCANTATION	: Command(command = "incantation", callback = Callback.incantation),
			C.FORK			: Command(command = "fork", callback = Callback.fork),
			C.EXPULSE		: Command(command = "expulse", callback = Callback.expulse),
			C.BROADCAST		: Command(command = "broadcast", callback = Callback.broadcast),
		}

	def	transceive(self, cmd):
		if self.cmd.state != None and self.cmd.state == S.NONE:
			command = cmd.command
			if self.cmd.buf != None:
				command = cmd.command + ' ' + self.cmd.buf
			#on push la query dans qtransceive
			self.qtransceive.append(command + '\n')
			#on set la command en cours
			self.cmd.update(id = self.cmd.id, command = cmd.command, callback = cmd.callback, state = S.APPENDED)

	def	death(self):
		self.alive = False
		self.qtransceive.clear()

	def	query(self):
		if self.cmd.id == C.DEATH:
			self.death()
		elif self.cmd.id in self.cmds:
			self.transceive(self.cmds[self.cmd.id])

	def	server_instructions(self):
		for i in range(len(self.qreceive.buf)):
			if "message " in self.qreceive.buf[i]:
				#server send a broadcast
				print("bot receive a broadcast")
			elif "deplacement " in self.qreceive.buf[i]:
				#server send a kick
				print("bot has been kick")
			elif "mort" in self.qreceive.buf[i]:
				#server send death
				print("bot has die")
				self.cmd.id = C.DEATH
				return
			else:
				#server respond to our query
				self.cmd.update(response = self.qreceive.buf[i], state = S.RECEIVED)

	def	callback(self):
		#si on a une reponse et un callback lié
		if self.cmd.response != None and self.cmd.state == S.RECEIVED:
			Callback.run(self.cmd)
			self.cmd.clean()

	def	run(self):
		#read server instructions
		self.server_instructions()
		#execute callback when there is a query's response
		self.callback()
		if self.cmd.state != S.PENDING:
			#make a query
			self.query()

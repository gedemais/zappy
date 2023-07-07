from callback import Callback
from command import C, S, Command


class	Bot:
	def __init__(self, client_data):
		self.alive = True
		self.qreceive = client_data.qreceive
		self.qtransceive = client_data.qtransceive
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

	def	transceive(self, _cmd, cmd):
		if _cmd.state != None and _cmd.state == S.NONE:
			command = cmd.command
			if _cmd.buf != None:
				command = cmd.command + ' ' + _cmd.buf
			#on push la query dans qtransceive
			self.qtransceive.append(command + '\n')
			#on set la command en cours
			_cmd.reset(id = _cmd.id, command = cmd.command, callback = cmd.callback, state = S.APPENDED)

	def	death(self):
		print("bot has die")
		self.alive = False
		self.qtransceive.clear()

	def	server_transceive(self, cmd):
		if cmd.id == C.DEATH:
			self.death()
		elif cmd.id in self.cmds:
			self.transceive(cmd, self.cmds[cmd.id])

	def	server_receive(self, cmd):
		for i in range(len(self.qreceive.buf)):
			if "message " in self.qreceive.buf[i]:
				#server send a broadcast
				Callback.broadcast_received()
			elif "deplacement " in self.qreceive.buf[i]:
				#server send a kick
				Callback.kick()
			elif "mort" in self.qreceive.buf[i]:
				#server send death
				cmd.reset(id = C.DEATH)
				return
			elif cmd.state != S.TRAITING:
				#server respond to our query
				cmd.update(response = self.qreceive.buf[i], state = S.RECEIVED)
		#use the bot to call the callback
		self.callback(cmd)

	def	callback(self, cmd):
		#si on a une reponse et un callback lié
		if cmd.response != None and cmd.state == S.RECEIVED:
			Callback.run(cmd)
			cmd.state = S.TRAITING

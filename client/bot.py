from command import C, S, Command


class	Bot:
	def __init__(self, client_data):
		self.alive = True
		self.qreceive = client_data.qreceive
		self.qtransceive = client_data.qtransceive
		self.cmds = {
			C.CONNECT_NBR	: Command(command = "connect_nbr"),
			C.INVENTAIRE	: Command(command = "inventaire"),
			C.VOIR			: Command(command = "voir"),
			C.AVANCE		: Command(command = "avance"),
			C.DROITE		: Command(command = "droite"),
			C.GAUCHE		: Command(command = "gauche"),
			C.PREND			: Command(command = "prend"),
			C.POSE			: Command(command = "pose"),
			C.INCANTATION	: Command(command = "incantation"),
			C.FORK			: Command(command = "fork"),
			C.EXPULSE		: Command(command = "expulse"),
			C.BROADCAST		: Command(command = "broadcast"),
		}

	def	transceive(self, _cmd, cmd):
		if _cmd.state != None and _cmd.state == S.NONE:
			command = cmd.command
			if _cmd.buf != None:
				command = cmd.command + ' ' + _cmd.buf
			#on push la query dans qtransceive
			self.qtransceive.append(command + '\n')
			#on set la command en cours
			_cmd.reset(id = _cmd.id, command = cmd.command, state = S.APPENDED)

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
				print("bot receive a broadcast")
				print(cmd.debug())
			elif "deplacement " in self.qreceive.buf[i]:
				#server send a kick
				print("bot has been kick")
				print(cmd.debug())
			elif "mort" in self.qreceive.buf[i]:
				#server send death
				cmd.reset(id = C.DEATH)
				return
			elif cmd.state != S.TRAITING:
				#server respond to our query
				cmd.update(response = self.qreceive.buf[i], state = S.RECEIVED)
		#si on a une reponse
		if cmd.response != None and cmd.state == S.RECEIVED:
			cmd.state = S.TRAITING

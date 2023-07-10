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

	def	death(self):
		print("bot has die")
		self.alive = False

	#push the cmd in a queue
	def	transceive(self, cmd, command):
		if cmd.state == S.CREATED:
			if cmd.buf != None:
				command += ' ' + cmd.buf
			#on push la query dans qtransceive
			self.qtransceive.append(command + '\n')
			cmd.state = S.TRANSCEIVED

	#append la cmd en cours dans qtransceive pour un envoit au server
	def	server_transceive(self, cmd):
		if cmd.id == C.DEATH:
			self.death()
		elif cmd.id in self.cmds:
			self.transceive(cmd, str(self.cmds[cmd.id].command))

	def	parse_response(self, cmd):
		response = cmd.response
		#parsing inventaire and voir
		if response[0] == '{' and response[-1] == '}':
			cmd.response = []
			response = response[1:-1]
			for tile in response.split(','):
				loots = tile.split(' ')
				while '' in loots:
					loots.remove('')
				cmd.response.append(loots)
			#parsing inventaire
			if "ttl" in response:
				response = {}
				for i in range(len(cmd.response)):
					response[cmd.response[i][0]] = int(cmd.response[i][1])
				cmd.response = response
			#parsing voir
			else:
				response = []
				for element in cmd.response:
					tmp = {}
					for elt in element:
						if elt in tmp:
							tmp[elt] += 1	
						else:
							tmp[elt] = 1
					response.append(tmp)
				cmd.response = response

	#parse les données du server fraichement reçues
	def	server_receive(self, cmd):
		for i in range(len(self.qreceive.buf)):
			if "message " in self.qreceive.buf[i]:
				#server send a broadcast
				print("bot receive a broadcast")
			elif "deplacement " in self.qreceive.buf[i]:
				#server send a kick
				print("bot has been kick")
			elif "mort" in self.qreceive.buf[i]:
				#server send death
				self.death()
				return
			elif cmd.state == S.PENDING and len(self.qreceive.buf[i]) > 0:
				#server send our response
				cmd.response = self.qreceive.buf[i]
				cmd.state = S.RECEIVED
				self.parse_response(cmd)

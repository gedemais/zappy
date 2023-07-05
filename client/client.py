moves = ['avance\n', 'avance\n', 'avance\n', 'avance\n', 'droite\n']

class	Client:
	def __init__(self, host, port, team_name, s):
		self.host = host
		self.port = port
		self.team_name = team_name
		self.s = s

	def send(self, cmd):
		self.s.send(bytes(cmd.encode("utf-8")))
		reply = self.s.recv(1024).decode("utf-8")
		print(cmd.strip(), reply.strip())

	def connect_to_server(self):
		self.s.connect((self.host, self.port))
		reply = self.s.recv(1024).decode("utf-8")
		print(reply) # BIENVENUE
		self.send(self.team_name)

	def	connect_nbr(self):
		self.send("connect_nbr")

	def	voir(self):
		self.send("voir")

	def	inventaire(self):
		self.send("inventaire")

	def	broadcast(self, message):
		self.send("broadcast " + message)

	def	move(self, i):
		if i == 0:
			self.connect_nbr()
			self.voir()
			self.inventaire()
		self.send(moves[i])

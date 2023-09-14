import socket, sys

from utils.bot import Bot
from utils.command import S
from utils.queue import Queue


#on utilise une queue pour stocker les cmds a envoyer
#on utilise une queue pour stocker les réponses / messages du servers
#ces deux queues sont traitées dans le bot afin de gérer la reception et l'envois des cmds emisent par brain
class	Client:
	def __init__(self, host, port, team_name):
		self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.host = host
		self.port = port
		self.team_name = team_name
		self.wx, self.wy = 0, 0
		self.qreceive = Queue()
		self.qtransceive = Queue()
		self.bot = Bot(self)
		self.connected = False

	#connect to server and return world size
	def connect(self):
		try:
			self.s.connect((self.host, self.port))
		except:
			print("client connect problem")
			sys.exit(1)
			return 0, 0
		self.connected = True
		try:
			response = self.s.recv(1024).decode("utf-8")
			self.s.send(bytes(self.team_name.encode("utf-8")))
			response = self.s.recv(1024).decode("utf-8")
		except:
			print("client init problem")
			self.connected = False
			sys.exit(1)
			return 0, 0

		#world size
		wsize = [0, 0]
		split = response.split('\n')
		if len(split) == 3:
			wsize = split[1].split(' ')
			self.wx, self.wy = wsize[0], wsize[1]
		return int(wsize[0]), int(wsize[1])

	def	close(self):
		self.connected = False
		self.s.close()

	#receive data from server
	def	receive(self, cmd):
		self.qreceive.reset()
		server_messages = []
		self.s.settimeout(0.1)
		while True:
			data = None
			try:
				data = self.s.recv(8192)
			except socket.timeout:
				break
			except:
				print("client receive problem")
				self.connected = False
			if not data or data is None:
				self.connected = False
				return None
			split = data.decode("utf-8").split('\n')
			for message in split:
				if len(message) > 0:
					self.qreceive.append(message)
		if len(self.qreceive.buf) > 0:
			#use the bot to parse qreceive and check if the server sent us a message
			#(broadcast, kick, death, game didn't start yet)
			server_messages = self.bot.server_receive(cmd)
		return server_messages

	#transceive data to server
	def transceive(self, cmd):
		if cmd.id != None:
			#use the bot to prepare qtransceive
			self.bot.server_transceive(cmd)
			#si le cmd.state a été push dans la queue et qu'une queue existe
			if cmd.state == S.TRANSCEIVED:
				for command in self.qtransceive.buf:
					try:
						self.s.send(bytes(command.encode("utf-8")))
					except:
						self.connected = False
				self.qtransceive.reset()
				#cmd has been sent so state is now pending
				cmd.state = S.PENDING

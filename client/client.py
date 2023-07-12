import socket

from utils.bot import Bot
from utils.command import S
from utils.queue import Queue


#on utilise une queue pour stocker les cmds a envoyer
#on utilise une queue pour stocker les réponses / messages du servers
#ces deux queues sont traitées dans le bot afin de gérer la reception et l'envois des cmds emisent par brain
class	Client:
	def __init__(self, host, port, team_name, s):
		self.host = host
		self.port = port
		self.team_name = team_name
		self.s = s
		self.wx, self.wy = 0, 0
		self.qreceive = Queue()
		self.qtransceive = Queue()
		self.bot = Bot(self)

	#connect to server and return world size
	def connect(self):
		#world size
		wsize = [0, 0]
		self.s.connect((self.host, self.port))
		#todo protéger les appels servers
		if self.s == None:
			return wsize[0], wsize[1]
		reply = self.s.recv(1024).decode("utf-8")
		if reply == None:
			return wsize[0], wsize[1]
		print(reply.strip()) # BIENVENUE
		self.s.send(bytes(self.team_name.encode("utf-8")))
		reply = self.s.recv(1024).decode("utf-8")
		if reply == None:
			return wsize[0], wsize[1]
		print(reply.strip())
		split = reply.split('\n')
		#split[0] "int" - split[1] "wx, wy"
		if len(split) == 3:
			wsize = split[1].split(' ')
			self.wx, self.wy = wsize[0], wsize[1]
		return int(wsize[0]), int(wsize[1])

	#receive data from server
	def	receive(self, cmd):
		self.qreceive.reset()
		#WIP mb better with none blockant socket
		self.s.settimeout(0.1)
		while True:
			try:
				data = self.s.recv(1024)
			except socket.timeout:
				break
			if not data:
				break
			split = data.decode("utf-8").split('\n')
			for i in range(len(split)):
				if len(split[i]) > 0:
					self.qreceive.append(split[i])
		if len(self.qreceive.buf) > 0:
			#use the bot to parse qreceive
			self.bot.server_receive(cmd)

	#transceive data to server
	def transceive(self, cmd):
		if cmd.id != None:
			#use the bot to prepare qtransceive
			self.bot.server_transceive(cmd)
			buf_len = len(self.qtransceive.buf)
			#si le cmd.state a été push dans la queue et qu'une queue existe
			if cmd.state == S.TRANSCEIVED and buf_len > 0:
				for i in range(buf_len):
					self.s.send(bytes(self.qtransceive.buf[i].encode("utf-8")))
				self.qtransceive.reset()
				#cmd has been sent so state is now pending
				cmd.state = S.PENDING

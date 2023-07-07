import socket

from bot import Bot
from command import S
from queue import Queue


class	Client:
	def __init__(self, host, port, team_name, s):
		self.host = host
		self.port = port
		self.team_name = team_name
		self.s = s
		self.qreceive = Queue()
		self.qtransceive = Queue()
		self.bot = Bot(self)

	def connect(self):
		self.s.connect((self.host, self.port))
		reply = self.s.recv(1024).decode("utf-8")
		print(reply.strip()) # BIENVENUE
		self.s.send(bytes(self.team_name.encode("utf-8")))
		reply = self.s.recv(1024).decode("utf-8")
		print(reply.strip())

	def	receive(self, cmd):
		self.qreceive.clear()
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
		if len(self.qreceive.buf):
			print("receive:", self.qreceive.buf)
		#use the bot to parse reception
		self.bot.server_receive(cmd)

	def transceive(self, cmd):
		#use the bot to prepare qtransceive
		self.bot.server_transceive(cmd)
		buf_len = len(self.qtransceive.buf)

		if buf_len > 0:
			print("transceive:", self.qtransceive.buf)
			for i in range(buf_len):
				self.s.send(bytes(self.qtransceive.buf[i].encode("utf-8")))
			self.qtransceive.clear()
			#set a pending state one the query
			cmd.state = S.PENDING

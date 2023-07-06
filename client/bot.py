from enum import Enum
from callback import Callback


moves = ["avance\n", "avance\n", "avance\n", "avance\n", "droite\n"]

class	T(Enum):
	DEATH		= 0
	CONNECT_NBR	= 1
	INVENTAIRE	= 2
	VOIR		= 3

tasks = {
	T.DEATH			: None,
	T.CONNECT_NBR	: { "command" : "connect_nbr\n", "callback" : Callback.connect_nbr },
	T.INVENTAIRE	: { "command" : "inventaire\n", "callback" : Callback.inventaire },
	T.VOIR			: { "command" : "voir\n", "callback" : Callback.voir },
}

class	Bot:
	def __init__(self, client_data):
		self.alive = True
		self.task_id = T.CONNECT_NBR
		self.qreceive = client_data.qreceive
		self.qtransceive = client_data.qtransceive
		self.task = { "command" : None, "response" : None, "callback" : None }
		self.switch = {
			T.DEATH			: self.death,
			T.CONNECT_NBR	: self.connect_nbr,
			T.INVENTAIRE	: self.inventaire,
			T.VOIR			: self.voir,
		}

	def	transceive(self, task):
		self.qtransceive.append(task["command"])
		self.task["command"] = task["command"]
		self.task["callback"] = task["callback"]
	
	def	death(self):
		self.alive = False
		self.qtransceive.clear()

	def	connect_nbr(self):
		self.transceive(tasks[T.CONNECT_NBR])

	def	inventaire(self):
		self.transceive(tasks[T.INVENTAIRE])

	def	voir(self):
		self.transceive(tasks[T.VOIR])

	def	manager(self, task_id):
		if task_id in self.switch:
			self.switch[task_id]()
		else:
			print("Default task")

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
				self.task_id = T.DEATH
				return
			else:
				self.task["response"] = self.qreceive.buf[i]

	def	run(self):
		self.server_instructions()
		Callback.run(self.task)
		self.manager(self.task_id)

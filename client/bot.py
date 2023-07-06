from callback import Callback
from task import T, task_constructor


class	Bot:
	def __init__(self, client_data):
		self.alive = True
		self.qreceive = client_data.qreceive
		self.qtransceive = client_data.qtransceive
		self.task = task_constructor(id = T.CONNECT_NBR)
		self.tasks = {
			T.CONNECT_NBR	: task_constructor(command = "connect_nbr", callback = Callback.connect_nbr),
			T.INVENTAIRE	: task_constructor(command = "inventaire", callback = Callback.inventaire),
			T.VOIR			: task_constructor(command = "voir", callback = Callback.voir),
			T.AVANCE		: task_constructor(command = "avance", callback = Callback.avance),
			T.DROITE		: task_constructor(command = "droite", callback = Callback.droite),
			T.GAUCHE		: task_constructor(command = "gauche", callback = Callback.gauche),
			T.PREND			: task_constructor(command = "prend", callback = Callback.prend),
			T.POSE			: task_constructor(command = "pose", callback = Callback.pose),
			T.INCANTATION	: task_constructor(command = "incantation", callback = Callback.incantation),
			T.FORK			: task_constructor(command = "fork", callback = Callback.fork),
			T.EXPULSE		: task_constructor(command = "expulse", callback = Callback.expulse),
			T.BROADCAST		: task_constructor(command = "broadcast", callback = Callback.broadcast),
		}

	def	transceive(self, task):
		self.task = task_constructor(self.task["id"], task["command"], None, task["callback"], task["buf"])
		command = task["command"]
		if task["buf"] != None:
			command = task["command"] + ' ' + task["buf"]
		self.qtransceive.append(command + '\n')
	
	def	death(self):
		self.alive = False
		self.qtransceive.clear()

	def	manager(self, task_id):
		if task_id == T.DEATH:
			self.death()
		elif task_id in self.tasks:
			self.transceive(self.tasks[task_id])
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
				self.task["id"] = T.DEATH
				return
			else:
				#server respond to our query
				self.task["response"] = self.qreceive.buf[i]

	def	callback(self):
		Callback.run(self.task)
		self.task = task_constructor(id = self.task["id"])

	def	run(self):
		self.server_instructions()
		self.callback()
		self.manager(self.task["id"])

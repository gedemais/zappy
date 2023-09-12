from utils.command import C, S, Command
from utils.queue import Queue


class	Brain:
	def	__init__(self):
		self.cmd = Command()
		self.queue = Queue()
		self.busy = False
		self.memory = []

	#input a Command or an array of Commands
	#start the brain process if commands is valid
	def	input(self, commands):
		self.memory = []
		if commands != None and len(commands) > 0:
			self.busy = True
			self.queue.start(commands)
			command = self.queue.get_state_elt()
			self.cmd = command.copy()

	def	reset(self):
		self.cmd.reset()
		self.queue.running == False
		self.queue.end()
		self.busy = False
		self.memory = []

	def	parse_cmd(self, cmd):
		if cmd == None or hasattr(cmd, "id") == False:
			return False
		if type(cmd.id) != type(C.DEATH):
			return False
		return True

	#only S.NONE or S.RECEIVED cmd here
	def	action(self):
		#si RECEIVED alors on avancce dans la queue de command
		if self.cmd.state == S.RECEIVED:
			#on stock la response de la command dans un tableau
			self.memory.append(self.cmd.copy())
			self.queue.update(1)
		#on recupere l'element actuel de la queue de command
		cmd = self.queue.get_state_elt()
		#si la cmd est valide
		if self.parse_cmd(cmd) == True:
			#on set une nouvelle cmd à transceive
			self.cmd.reset(id = cmd.id, buf = cmd.buf, state = S.CREATED)
		else:
			#erreur on stop le brain process
			self.queue.running = False
			self.cmd.reset()

	#end the brain process
	def	end(self):
		self.queue.end()
		if self.queue.running == False:
			self.busy = False

	#transform le tableau de commands input en commande individuelle à transceive
	#process est busy lorsqu'il traite une série de commandes
	#quand une commande est received elle est stock dans brain.memory
	#return True if busy
	def	process(self):
		if self.cmd.state is not S.NONE:
			# print("BRAIN", self.cmd.debug())
			pass
		if self.busy == False:
			return False
		#brain only trait none created or received cmds
		if self.cmd.state != S.NONE and self.cmd.state != S.RECEIVED:
			return self.busy
		self.action()
		self.end()
		return self.busy

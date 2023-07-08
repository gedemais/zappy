from command import C, S, Command
from queue import Queue


class	Brain:
	def	__init__(self):
		self.cmd = Command()
		self.queue = Queue()
		self.busy = False
		self.commands = []

	def	parse_cmd(self, cmd):
		if cmd == None or hasattr(cmd, "id") == False:
			return False
		if type(cmd.id) != type(C.DEATH):
			return False
		return True

	def	action(self):
		#si RECEIVED alors on passe au state suivant
		if self.cmd.state == S.RECEIVED:
			self.queue.update(1)
		self.cmd.reset()
		cmd = self.queue.get_state_elt()
		if self.parse_cmd(cmd) == True:
			#on set une nouvelle cmd à transceive
			self.cmd.reset(id = cmd.id, buf = cmd.buf, state = S.CREATED)
		else:
			self.queue.running = False

	def	end(self):
		self.queue.end()
		if self.queue.running == False:
			self.busy = False

	def	input(self, commands):
		self.busy = True
		self.queue.start(commands)

	#return True if busy
	def	process(self, state = False):
		print("brain", self.cmd.debug())
		if self.busy == False:
			return False
		if state == True:
			return self.busy
		if self.cmd.state != S.NONE and self.cmd.state != S.RECEIVED:
			return self.busy
		self.action()
		self.end()
		return self.busy

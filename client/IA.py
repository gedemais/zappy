from command import C, S, Command
from queue import Queue

_commands_state = {
	C.DEATH			: S.NONE,
	C.CONNECT_NBR	: S.NONE,
	C.INVENTAIRE	: S.NONE,
	C.VOIR			: S.NONE,
	C.AVANCE		: S.NONE,
	C.DROITE		: S.NONE,
	C.GAUCHE		: S.NONE,
	C.PREND			: S.NONE,
	C.POSE			: S.NONE,
	C.INCANTATION	: S.NONE,
	C.FORK			: S.NONE,
	C.EXPULSE		: S.NONE,
	C.BROADCAST		: S.NONE
}

class	IA:
	def	__init__(self):
		self.name = "bernard"
		self.cmd = Command()
		self.queue = Queue()
		self.busy = False

	def	action(self, cmd):
		self.cmd.clean()
		self.cmd.id = cmd.id
		self.cmd.buf = cmd.buf

	def	start(self, queue):
		if self.busy == False and queue != None and len(queue) > 0:
			self.busy = True
			self.queue.start(queue)
	
	def	end(self):
		self.queue.end()
		if self.queue.running == False:
			self.queue.clear()
			self.busy = False	

	#return True if busy
	def	interact(self, queue = None):
		self.start(queue)
		if self.busy == False:
			return False
		if self.cmd.state != S.NONE and self.cmd.state != S.TRAITING:
			return self.busy
		state = self.queue.state
		cmd = self.queue.buf[state]
		if self.cmd.state == S.NONE:
			self.action(cmd)
		if self.cmd.state == S.TRAITING:
			self.action(cmd)
			self.queue.update(1)
		self.end()
		return self.busy

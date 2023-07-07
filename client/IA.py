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
		_commands_state[cmd.id] = S.PENDING
		if cmd.buf != None:
			self.cmd.buf = cmd.buf

	#return True if busy
	def	interact(self, queue = None):
		if self.busy == False and queue != None:
			print("interact busy")
			self.busy = True
			self.queue.start(queue)
		if self.busy == False:
			print("interact not busy")
			return self.busy
		if self.cmd.state != S.NONE and self.cmd.state != S.TRAITING:
			return self.busy
		state = self.queue.state
		cmd = self.queue.buf[state]
		if _commands_state[cmd.id] == S.NONE:
			self.action(cmd)
		if self.cmd.state != S.TRAITING:
			return self.busy
		if _commands_state[cmd.id] == S.RECEIVED:
			_commands_state[cmd.id] = S.NONE
			self.queue.update(1)
		self.queue.end()
		if self.queue.running == False:
			self.queue.clear()
			self.busy = False
		return self.busy

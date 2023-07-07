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
	
	def	square(self):
		self.queue.append([
			Command(id = C.AVANCE),
			Command(id = C.AVANCE),
			Command(id = C.AVANCE),
			Command(id = C.DROITE),
		])

	def	prend(self):
		self.queue.append(Command(id = C.PREND, buf = "nourriture"))

	def	action(self, cmd):
		self.cmd.clean()
		self.cmd.id = cmd.id
		_commands_state[cmd.id] = S.PENDING
		if cmd.buf != None:
			self.cmd.buf = cmd.buf

	#return True if busy
	def	interact(self):
		if self.cmd.state != S.NONE and self.cmd.state != S.TRAITING:
			return True
		self.queue.start()
		state = self.queue.state
		cmd = self.queue.buf[state]

		if _commands_state[cmd.id] == S.NONE:
			self.action(cmd)
		if self.cmd.state != S.TRAITING:
			return
		if _commands_state[cmd.id] == S.RECEIVED:
			_commands_state[cmd.id] = S.NONE
			self.queue.update(1)

		self.queue.end()
		return self.queue.running
	
	def	use(self):
		print("use")
		

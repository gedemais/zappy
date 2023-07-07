from enum import Enum


class	C(Enum):
	DEATH		= 0
	CONNECT_NBR	= 1
	INVENTAIRE	= 2
	VOIR		= 3
	AVANCE		= 4
	DROITE		= 5
	GAUCHE		= 6
	PREND		= 7
	POSE		= 8
	INCANTATION	= 9
	FORK		= 10
	EXPULSE		= 11
	BROADCAST	= 12

class	S(Enum):
	NONE		= 0
	APPENDED	= 1
	PENDING		= 2
	RECEIVED	= 3
	TRAITING	= 4

class	Command:
	def	__init__(self, id = None, command = None, response = None, callback = None, buf = None, state = S.NONE):
		self.id = id
		self.command = command
		self.response = response
		self.callback = callback
		self.buf = buf
		self.state = state

	def	update(self, id = None, command = None, response = None, callback = None, buf = None, state = None):
		self.id = id = id if id != None else self.id
		self.command = command if command != None else self.command
		self.response = response if response != None else self.response
		self.callback = callback if callback != None else self.callback
		self.buf = buf if buf != None else self.buf
		self.state = state if state != None else self.state

	def	reset(self, id = None, command = None, response = None, callback = None, buf = None, state = S.NONE):
		self.id = id
		self.command = command
		self.response = response
		self.callback = callback
		self.buf = buf
		self.state = state

	def	clean(self):
		self.command = None
		self.response = None
		self.callback = None
		self.state = S.NONE

	def	data(self):
		return {
			"id"		: self.id,
			"command"	: self.command,
			"response"	: self.response,
			"callback"	: self.callback,
			"buf"		: self.buf,
			"state"		: self.state
		}

	def	debug(self):
		return {
			"id"		: self.id,
			"command"	: self.command,
			"state"		: self.state
		}

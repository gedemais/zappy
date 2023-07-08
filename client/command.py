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
	CREATED		= 1
	TRANSCEIVED	= 2
	PENDING		= 3
	RECEIVED	= 4

class	Command:
	def	__init__(self, id = None, command = None, response = None, buf = None, state = S.NONE):
		self.id = id
		self.command = command
		self.response = response
		self.buf = buf
		self.state = state

	def	reset(self, id = None, command = None, response = None, buf = None, state = S.NONE):
		self.id = id
		self.command = command
		self.response = response
		self.buf = buf
		self.state = state

	def	copy(self):
		return(Command(
			id = self.id,
			command = self.command,
			response = self.response,
			buf	= self.buf,
			state = self.state
		))

	def	data(self):
		return {
			"id"		: self.id,
			"command"	: self.command,
			"response"	: self.response,
			"buf"		: self.buf,
			"state"		: self.state
		}

	def	debug(self):
		return {
			"id"		: self.id,
			"state"		: self.state
		}

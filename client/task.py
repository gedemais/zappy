from enum import Enum


class	T(Enum):
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

def	task_constructor(id = None, command = None, response = None, callback = None, buf = None):
	return {
		"id"		: id,
		"command"	: command,
		"response"	: response,
		"callback"	: callback,
		"buf"		: buf
	}

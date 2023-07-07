from command import C, S
from IA	import _commands_state


class	Callback:
	def	connect_nbr(cmd):
		_commands_state[C.CONNECT_NBR] = S.RECEIVED

	def	inventaire(cmd):
		_commands_state[C.INVENTAIRE] = S.RECEIVED

	def	voir(cmd):
		_commands_state[C.VOIR] = S.RECEIVED

	def	avance(cmd):
		_commands_state[C.AVANCE] = S.RECEIVED

	def	droite(cmd):
		_commands_state[C.DROITE] = S.RECEIVED

	def	gauche(cmd):
		_commands_state[C.GAUCHE] = S.RECEIVED

	def	prend(cmd):
		_commands_state[C.PREND] = S.RECEIVED

	def	pose(cmd):
		_commands_state[C.POSE] = S.RECEIVED

	def	incantation(cmd):
		_commands_state[C.INCANTATION] = S.RECEIVED

	def	fork(cmd):
		_commands_state[C.FORK] = S.RECEIVED

	def	expulse(cmd):
		_commands_state[C.EXPULSE] = S.RECEIVED

	def	broadcast(cmd):
		_commands_state[C.BROADCAST] = S.RECEIVED

	def	broadcast_received(cmd):
		print("bot receive a broadcast")
		print(cmd.debug())

	def	kick(cmd):
		print("bot has been kick")
		print(cmd.debug())

	def	run(cmd):
		cmd.callback(cmd)

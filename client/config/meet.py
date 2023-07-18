from utils.command import C
from action.callback import compute_action, is_blind


class	Meet:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		compute_action(bernard, C.BROADCAST, 1, "Hello world !")

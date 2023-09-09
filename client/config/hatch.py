from utils.command import C
from action.utils import compute_action, is_blind


class	Hatch:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		compute_action(bernard, C.FORK)

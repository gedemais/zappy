from utils.command import C
from action.utils import compute_action, is_blind


class	Hatch:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if bernard.team_total == 6:
			return
		compute_action(bernard, C.FORK)

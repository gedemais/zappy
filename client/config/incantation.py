from utils.command import C
from action.utils import compute_action, is_blind
from action.incant import incant_possible, incant_put
from config.meet import handle_food


def		drop_requirement(bernard):
	#incant_put return un array contenant les ressources à poser afin d'incanter
	needs_to_put = incant_put(bernard)
	for item in needs_to_put:
		if "player" in item:
			continue
		if needs_to_put[item] > 0:
			compute_action(bernard, C.POSE, needs_to_put[item], item)

class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if  bernard.lvl > 8:
			return
		handle_food(bernard)
		if incant_possible(bernard, False) == True:
			drop_requirement(bernard)
			compute_action(bernard, C.INCANTATION)
			compute_action(bernard, C.VOIR)
			compute_action(bernard, C.CONNECT_NBR)

from utils.command import C
from action.callback import compute_action, is_blind
from action.incant import incant_possible, incant_put


def		drop_requirement(bernard):
	print("preparing the ritual...")
	#incant_put return un array contenant les ressources à poser afin d'incanter
	needs_to_put = incant_put(bernard)
	for item in needs_to_put:
		if "player" in item:
			continue
		if needs_to_put[item] > 0:
			print("placing {} {}".format(needs_to_put[item], item))
			compute_action(bernard, C.POSE, needs_to_put[item], item)

class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if  bernard.lvl > 8 or bernard.inventory["nourriture"] < 5:
			return
		if incant_possible(bernard.inventory, bernard.lvl) == True:
			drop_requirement(bernard)
			print("I'm Elevating \o/ !")
			compute_action(bernard, C.INCANTATION)

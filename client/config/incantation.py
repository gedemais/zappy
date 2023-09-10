from utils.command import C
from action.utils import compute_action, is_blind
from action.incant import incant_possible, incant_put
from action.view import view_index


def		collect_food(bernard, nb_food_max):
	#collecting foods
	bernardindex = view_index(bernard.x, bernard.y)
	viewcase = bernard.view[bernardindex]
	if "nourriture" in viewcase and viewcase["nourriture"] > 0:
		nb_food = viewcase["nourriture"]
		if nb_food > nb_food_max:
			nb_food = nb_food_max
		print("looting {} nourriture".format(nb_food))
		compute_action(bernard, C.PREND, nb_food, "nourriture")
		viewcase["nourriture"] -= nb_food

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

def		handle_food(bernard):
	if bernard.inventory["nourriture"] < 15:
		collect_food(bernard, 5)
	elif bernard.inventory["nourriture"] < 20:
		collect_food(bernard, 3)
	if bernard.inventory["nourriture"] > 25\
			and "player" in bernard.view[0] and bernard.view[0]["player"] > 1:
		value = bernard.inventory["nourriture"] - 20
		p = 40
		nb_food = 1 + int((value * p) / 100)
		compute_action(bernard, C.POSE, nb_food, "nourriture")
		print("giving {} food to leader".format(nb_food))

class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if  bernard.lvl > 8:
			return
		handle_food(bernard)
		if incant_possible(bernard, True) == True:
			drop_requirement(bernard)
			print("I'm Elevating \o/ !")
			compute_action(bernard, C.INCANTATION)
			compute_action(bernard, C.VOIR)

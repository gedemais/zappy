from utils.command import C
from action.utils import compute_action, is_blind, send_broadcast
from action.view import view_index


def		collect_food(bernard, viewcase, drop = False):
	nb_food = 0
	if drop == True:
		nb_food = bernard.inventory["nourriture"] - 20
	elif "nourriture" in viewcase and viewcase["nourriture"] > 0:
		nb_food = 1 + int(25 * (1 / bernard.inventory["nourriture"]))
		if nb_food > viewcase["nourriture"]:
			nb_food = viewcase["nourriture"]
	if nb_food > 5:
		nb_food = 5
	if drop == True and bernard.lvl < 7:
		compute_action(bernard, C.POSE, nb_food, "nourriture")
		if "nourriture" in viewcase and viewcase["nourriture"] > 0:
			viewcase["nourriture"] += nb_food
		else:
			viewcase["nourriture"] = nb_food
		bernard.inventory["nourriture"] -= nb_food
	elif "nourriture" in viewcase and viewcase["nourriture"] > 0:
		compute_action(bernard, C.PREND, nb_food, "nourriture")
		viewcase["nourriture"] -= nb_food
		bernard.inventory["nourriture"] += nb_food

def		handle_food(bernard):
	bernardindex = view_index(bernard.x, bernard.y)
	viewcase = bernard.view[bernardindex]

	if "player" not in viewcase or viewcase["player"] == 1:
		return
	if bernard.inventory["nourriture"] < 20:
		collect_food(bernard, viewcase)
	if "nourriture" in viewcase and viewcase["nourriture"] > 30\
			or bernard.leader == -1:
		return
	if bernard.inventory["nourriture"] > 25:
		collect_food(bernard, viewcase, True)

class	Meet:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return

		if bernard.leader == -1: # and bernard.t - bernard.last_broadcast > 100:
			send_broadcast(bernard, "Come to your leader !")
			handle_food(bernard)
			compute_action(bernard, C.VOIR, 1)
			return

		dir = bernard.leader

		bernardindex = view_index(bernard.x, bernard.y)
		if dir == 0 and "player" in bernard.view[bernardindex] and bernard.view[bernardindex]["player"] > 1:
			handle_food(bernard)
			compute_action(bernard, C.VOIR, 1)
			send_broadcast(bernard, "My level is : {}".format(bernard.lvl))
			return

		# 1 is front
		# 2 1 8
		# 3 X 7
		# 4 5 6
		if dir == 1 or dir == 2 or dir == 8:
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
		elif dir == 7:
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
		if dir == 5 or dir == 4 or dir == 6:
			#go back
			compute_action(bernard, C.GAUCHE, 2)
			compute_action(bernard, C.AVANCE, 1)
			bernard.y -= 1
		elif dir == 3:
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1

		bernard.leader = 0

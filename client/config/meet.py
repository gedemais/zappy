from utils.command import C
from action.utils import compute_action, is_blind, send_broadcast
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

class	Meet:
	def	__init__(self):
		pass

	#format: deplacement <K>
	# 1 is front
	# 2 1 8
	# 3 X 7
	# 4 5 6
	def	run(bernard):
		if is_blind(bernard) == True:
			return

		#leader assignation
		if bernard.leader is None:
			bernard.leader = -1
			send_broadcast(bernard, "Come to your leader !")
		#establishing rally point
		if bernard.leader == -1 and bernard.t - bernard.last_broadcast > 100:
			send_broadcast(bernard, "Come to your leader !")
			print("Calling team mates to rush lvl 8 !")
			#leader food state
			if bernard.inventory["nourriture"] < bernard.foodmax:
				send_broadcast(bernard, "I need food my minions !")
				print("Asking team mates for foods !")
		if bernard.leader == -1:
			#refreshing vision
			compute_action(bernard, C.VOIR, 1)
			collect_food(bernard, 5)
			return

		dir = bernard.leader

		print("================================== DIR [ {} ]".format(dir))

		if dir == 0:
			compute_action(bernard, C.VOIR, 1)
			if bernard.inventory["nourriture"] < bernard.foodmin + 5:
				collect_food(bernard, 1)
			return
		
		#collecting foods
		collect_food(bernard, 1)

		if dir == 1 or dir == 2 or dir == 8:
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
			print("going front")
		elif dir == 7:
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			print("going right")
			#rotation so view update
			compute_action(bernard, C.VOIR, 1)
		if dir == 5 or dir == 4 or dir == 6:
			#go back
			compute_action(bernard, C.GAUCHE, 2)
			compute_action(bernard, C.AVANCE, 1)
			print("going back")
			#rotation so view update
			compute_action(bernard, C.VOIR, 1)
		elif dir == 3:
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			print("going left")
			#rotation so view update
			compute_action(bernard, C.VOIR, 1)

		bernard.leader = 0

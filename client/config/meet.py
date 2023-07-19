from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_index, outofview


class	Meet:
	def	__init__(self):
		pass

	#format: deplacement <K>
	# 1 is back
	# 2 1 8
	# 3 X 7
	# 4 5 6
	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if bernard.leader is None or bernard.leader == -1 \
				and bernard.t - bernard.last_broadcast > 1000:
			compute_action(bernard, C.BROADCAST, 1, "Come to your leader !")
			bernard.leader = -1
			print("Calling team mates to rush lvl 8 !")
			return

		#WIP si leader est set c'est une direction et il faut s'en rapprocher

		# #si on y voit rien on ne peut pas ramasser de nourriture
		# if outofview(bernard, bernard.x, bernard.y) == True:
		# 	compute_action(bernard, C.VOIR, 1)
		# 	return
		# #on recupere de la nourriture
		# viewcase = bernard.view[view_index(bernard.x, bernard.y)]
		# if "nourriture" in viewcase and viewcase["nourriture"] > 0:
		# 	compute_action(bernard, C.PREND, viewcase["nourriture"], "nourriture")
		# 	viewcase["nourriture"] -= viewcase["nourriture"]

		dir = bernard.leader

		print("================================== DIR [ {} ]".format(dir))

		# if dir == 0 or ("player" in viewcase and viewcase["player"] > 1):
		if dir == 0:
			compute_action(bernard, C.BROADCAST, 1, "I'm at destination !")
			compute_action(bernard, C.VOIR, 1)
			return

		if dir == 1:
			#go back
			compute_action(bernard, C.GAUCHE, 2)
			compute_action(bernard, C.AVANCE, 1)
			bernard.y -= 1
			print("going back")
		elif dir == 2:
			print("diagonale")
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
			print("going right")
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
			print("going right")
			pass
		elif dir == 3:
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
			print("going right")
		elif dir == 4:
			print("diagonale")
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
			print("going front")
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
			print("going right")
		if dir == 5:
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
			print("going front")
		elif dir == 6:
			print("diagonale")
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
			print("going front")
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1
			print("going left")
		elif dir == 7:
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1
			print("going left")
		elif dir == 8:
			print("diagonale")
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1
			print("going left")
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1
			print("going left")

		#reverse
		# if dir == 1:
		# 	#go front
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.y += 1
		# 	print("going front")
		# elif dir == 2:
		# 	print("diagonale")
		# 	#go front
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.y += 1
		# 	print("going front")
		# 	#go left
		# 	compute_action(bernard, C.GAUCHE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x -= 1
		# 	print("going left")
		# elif dir == 3:
		# 	#go left
		# 	compute_action(bernard, C.GAUCHE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x -= 1
		# 	print("going left")
		# elif dir == 4:
		# 	print("diagonale")
		# 	#go left
		# 	compute_action(bernard, C.GAUCHE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x -= 1
		# 	print("going left")
		# 	#go left
		# 	compute_action(bernard, C.GAUCHE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x -= 1
		# 	print("going left")
		# if dir == 5:
		# 	#go back
		# 	compute_action(bernard, C.GAUCHE, 2)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.y -= 1
		# 	print("going back")
		# elif dir == 6:
		# 	print("diagonale")
		# 	#go right
		# 	compute_action(bernard, C.DROITE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x += 1
		# 	print("going right")
		# 	#go right
		# 	compute_action(bernard, C.DROITE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x += 1
		# 	print("going right")
		# 	pass
		# elif dir == 7:
		# 	#go right
		# 	compute_action(bernard, C.DROITE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x += 1
		# 	print("going right")
		# elif dir == 8:
		# 	print("diagonale")
		# 	#go front
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.y += 1
		# 	print("going front")
		# 	#go right
		# 	compute_action(bernard, C.DROITE, 1)
		# 	compute_action(bernard, C.AVANCE, 1)
		# 	bernard.x += 1
		# 	print("going right")

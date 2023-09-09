from utils.command import C
from action.utils import compute_action, is_blind, send_broadcast


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
		if bernard.leader is None:
			bernard.leader = -1
			send_broadcast(bernard, "Come to your leader !")
			print("Calling team mates to rush lvl 8 !")
			return
		if bernard.leader == -1 and bernard.t - bernard.last_broadcast > 250:
			send_broadcast(bernard, "Come to your leader !")
			print("Calling team mates to rush lvl 8 !")
			compute_action(bernard, C.VOIR, 1)
		if bernard.leader == -1:
			return

		dir = bernard.leader

		print("================================== DIR [ {} ]".format(dir))

		if dir == 0:
			compute_action(bernard, C.VOIR, 1)
			return

		if dir == 1 or dir == 2 or dir == 8:
			#go front
			compute_action(bernard, C.AVANCE, 1)
			bernard.y += 1
			print("going front")
		elif dir == 3:
			#go left
			compute_action(bernard, C.GAUCHE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x -= 1
			print("going left")
		if dir == 5 or dir == 4 or dir == 6:
			#go back
			compute_action(bernard, C.GAUCHE, 2)
			compute_action(bernard, C.AVANCE, 1)
			bernard.y -= 1
			print("going back")
		elif dir == 7:
			#go right
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.AVANCE, 1)
			bernard.x += 1
			print("going right")

		bernard.leader = 0

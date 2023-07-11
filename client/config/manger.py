from utils.command import C
from action.callback import compute_action
from action.view import view_pos, view_find
from action.move import goto_pos


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				print("see nourriture ! going to pos: {}".format(targetindex))
				targetx, targety = view_pos(targetindex)
				goto_pos(bernard, targetx, targety)
				compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
				return
		else:
			#non : avancer
			print("no food in vision")
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.DROITE, 1)

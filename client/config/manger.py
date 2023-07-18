from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_pos, view_find
from action.move import goto_index


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		print("I'm hungry >< !")
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				print("{} nourriture ! going to pos: {}".format(loot["nourriture"], targetindex))
				goto_index(bernard, targetindex)
				compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
				loot["nourriture"] -= loot["nourriture"]
		else:
			#non : avancer
			print("no food nearby. going forward !")
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.VOIR, 1)

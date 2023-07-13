from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_pos, view_find
from action.move import goto_pos


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		print("I'm hungry >< !")
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, bernard.view, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				print("{} nourriture ! going to pos: {}".format(loot["nourriture"], targetindex))
				targetx, targety = view_pos(targetindex)
				goto_pos(bernard, targetx, targety)
				print("taking {} nourriture".format(loot["nourriture"]))
				compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
		else:
			#non : avancer
			print("no food nearby. going forward !'")
			compute_action(bernard, C.AVANCE, 2)
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.VOIR, 1)

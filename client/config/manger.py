from utils.command import C
from action.callback import compute_action
from action.view import view_pos, view_find
from action.move import goto_pos


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		print("I'm hungry !")
		if bernard.view == None or bernard.inventory == None \
				or len(bernard.view) == 0 or len(bernard.inventory) == 0:
			compute_action(bernard, C.VOIR, 1)
			compute_action(bernard, C.INVENTAIRE, 1)
			return
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				print("I see {} nourriture ! going to pos: {}".format(loot["nourriture"], targetindex))
				targetx, targety = view_pos(targetindex)
				goto_pos(bernard, targetx, targety)
				print("taking {} nourriture", loot["nourriture"])
				compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
		else:
			#non : avancer
			print("there is no food nearby. I'm going forward !'")
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.DROITE, 1)
			compute_action(bernard, C.VOIR, 1)

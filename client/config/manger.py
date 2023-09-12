from utils.command import C
from action.utils import compute_action, is_blind, send_broadcast, view_rand
from action.view import view_find
from action.move import goto_index


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		if bernard.leader == -1 or bernard.rushfinal == True:
			send_broadcast(bernard, "The king is dead, long live the king !")
			bernard.leader = None
			bernard.rushfinal = False
		print("I'm hungry >< !")
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				print("{} nourriture ! going to pos: {}".format(loot["nourriture"], targetindex))
				goto_index(bernard, targetindex)
				nb_loot = loot["nourriture"]
				if nb_loot > 6:
					nb_loot = 6
				compute_action(bernard, C.PREND, nb_loot, "nourriture")
				loot["nourriture"] -= nb_loot
		else:
			print("no food nearby. looking around !")
			view_rand(bernard)
			compute_action(bernard, C.VOIR, 1)

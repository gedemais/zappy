from utils.command import C
from action.utils import compute_action, is_blind, send_broadcast, set_dir
from action.view import view_find
from action.move import goto_index


class	Manger:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		bernard.rushfinal = False
		#y a t-il de la nourriture proche ?
		targetindex = view_find(bernard, "nourriture")
		if targetindex is not None:
			loot = bernard.view[targetindex]
			if "nourriture" in loot and loot["nourriture"] > 0:
				#oui : y aller et prendre
				goto_index(bernard, targetindex)
				nb_loot = loot["nourriture"]
				if nb_loot > 5:
					nb_loot = 5
				compute_action(bernard, C.PREND, nb_loot, "nourriture")
				loot["nourriture"] -= nb_loot
		else:
			set_dir(bernard)
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.VOIR, 1)

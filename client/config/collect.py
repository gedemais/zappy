from utils.command import C
from action.utils import compute_action, is_blind, set_dir, case_value
from action.view import view_distance, view_index, view_pos
from action.move import goto_path


def		farm_case(bernard, needs, index):
	view = bernard.view
	viewcase = view[index]

	for item in needs:
		if "player" not in item and needs[item] > 0:
			if item in view[index] and view[index][item] > 0:
				nb_loot = view[index][item]
				if nb_loot > 5:
					nb_loot = 5
				print("looting {} {}".format(nb_loot, item))
				compute_action(bernard, C.PREND, nb_loot, item)
				viewcase[item] -= nb_loot
	#on ramasse un peu de nourriture si il y en a
	if "nourriture" in view[index] and view[index]["nourriture"] > 0:
		nb_loot = view[index]["nourriture"]
		if nb_loot > 5:
			nb_loot = 5
		print("looting {} nourriture".format(nb_loot))
		compute_action(bernard, C.PREND, nb_loot, "nourriture")
		viewcase["nourriture"] -= nb_loot

def		check_neighbours(bernard, needs, ci):
	x, y = view_pos(ci)
	v = 0

	index = view_index(x - 1, y)
	if index < bernard.view_size:
		v += case_value(bernard.view[index], needs)
	index = view_index(x + 1, y)
	if index < bernard.view_size:
		v += case_value(bernard.view[index], needs)
	index = view_index(x, y - 1)
	if index < bernard.view_size:
		v += case_value(bernard.view[index], needs)
	index = view_index(x, y + 1)
	if index < bernard.view_size:
		v += case_value(bernard.view[index], needs)
	if v > 0:
		v = int(v / 2)
	elif v < 0:
		v = 0
	return v

#trouve la case opulante la plus proche
def		find_gorgeous_case(bernard, needs):
	bernardindex = view_index(bernard.x, bernard.y)
	index = None
	weight = 0

	#on parcourt le champ de vision
	for i in range(len(bernard.view)):
		tmp = case_value(bernard.view[i], needs)
		#si il y a des ressources needed sur la case ou bernard est alors on la collecte
		if i == 0 and i == bernardindex and tmp > 0:
			return i
		#plus la case est loin plus elle perd de la valeure
		if i > 8:
			tmp = tmp + check_neighbours(bernard, needs, i)
			tmp = tmp - view_distance(bernardindex, i) * 10
		if tmp < 0:
			tmp = 0
		if tmp > weight:
			index = i
			weight = tmp
	return index

class	Collect:
	def	__init__(self):
		pass

	def	run(bernard, needs):
		if is_blind(bernard) == True:
			return
		print("Collecting...")
		#looking for the best case around
		index = find_gorgeous_case(bernard, needs)

		if index is not None:
			goto_path(bernard, index)
			farm_case(bernard, needs, index)
		else:
			print("nothing found in view range")
			print("scooting the area")
			set_dir(bernard)
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.VOIR)

from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_find, view_distance, view_index
from action.move import goto_index


def		farm_case(bernard, needs, need):
	view = bernard.view

	for item in needs:
		if "player" not in item and needs[item] > 0:
			if item in view[need["index"]] and view[need["index"]][item] > 0:
				print("looting {} {}".format(view[need["index"]][item], item))
				compute_action(bernard, C.PREND, view[need["index"]][item], item)
	#on ramasse un peu de nourriture si il y en a
	if "nourriture" in view[need["index"]] and view[need["index"]]["nourriture"] > 0:
		print("looting {} nourriture".format(view[need["index"]]["nourriture"]))
		compute_action(bernard, C.PREND, view[need["index"]]["nourriture"], "nourriture")

#trouve la ressource dans nt la plus proche
def		find_closest_need(bernard, needs):
	need = { "item" : None, "index" : bernard.view_size }
	bernardindex = view_index(bernard.x, bernard.y)
	list_of_index = []
	#on recupere une list d'items needed et leurs index, présent dans le champ de vision
	for item in needs:
		if "player" not in item and needs[item] > 0:
			index = view_find(bernard, bernard.view, item)
			if index is not None:
				list_of_index.append({ "item" : item, "index" : index })
	#WIP ramasser le plus de loot needed
	#on recupere le loot le plus proche de bernard
	for target in list_of_index:
		print("found {} {} at {}".format(bernard.view[target["index"]][target["item"]], target["item"], target["index"]))
		if view_distance(bernardindex, target["index"]) < view_distance(bernardindex, need["index"]):
			need["item"] = target["item"]
			need["index"] = target["index"]
	return need

def		farm_ressources(bernard, needs):
	#looking for the needed loots
	find = False
	#WIP créer une fonction qui loot une liste d'objet
	#process un goto sur chaque pos en actualisant la pos de benard dans un tmp
	#pb: list de commands trop longue ?
	need = find_closest_need(bernard, needs)
	if need["item"] is not None:
		find = True
		print("going for {} at {}".format(need["item"], need["index"]))
		goto_index(bernard, need["index"])
		farm_case(bernard, needs, need)
	else:
		print("nothing found in view range")
	return find

class	Collect:
	def	__init__(self):
		pass

	def	run(bernard, needs):
		if is_blind(bernard) == True:
			return
		if farm_ressources(bernard, needs) == False:
			print("going forward !")
			compute_action(bernard, C.AVANCE, 2)
			compute_action(bernard, C.DROITE)
			print("scooting the area")
			compute_action(bernard, C.VOIR)

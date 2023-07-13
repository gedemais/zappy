from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_find, view_distance
from action.move import goto_index
from action.incant import incant_possible, incant_need, incant_put, incant_total


def		farm_case(bernard, n, nt, need):
	view = bernard.view

	print("looting {} {}".format(view[need["index"]][need["item"]], need["item"]))
	compute_action(bernard, C.PREND, bernard.view[need["index"]][need["item"]], need["item"])
	#on a ramassé une ressource donc on actualise la liste des needs
	n[need["item"]] -= view[need["index"]][need["item"]]
	nt[need["item"]] -= view[need["index"]][need["item"]]
	print("looking for other requirement")
	for item in nt:
		if "player" not in item and item != need["item"] and nt[item] > 0:
			if item in view[need["index"]] and view[need["index"]][item] > 0:
				print("looting {} {}".format(view[need["index"]][item], item))
				compute_action(bernard, C.PREND, view[need["index"]][item], item)
				#on actualise la liste des needs totaux
				nt[item] -= view[need["index"]][item]
	#on ramasse un peu de nourriture si il y en a
	if "nourriture" in view[need["index"]] and view[need["index"]]["nourriture"] > 0:
		print("looting {} nourriture".format(view[need["index"]]["nourriture"]))
		compute_action(bernard, C.PREND, view[need["index"]]["nourriture"], "nourriture")

#trouve la ressource dans n la plus proche
#WIP trouver la case avec le plus de ressources pour nt
def		find_closest_need(bernard, needs_to_incant):
	need = { "item" : None, "index" : bernard.view_size }

	for item in needs_to_incant:
		if "player" not in item and needs_to_incant[item] > 0:
			targetindex = view_find(bernard, bernard.view, item)
			if targetindex is not None:
				print("found {} {} at {}".format(bernard.view[targetindex][item], item, targetindex))
				if view_distance(0, targetindex) < view_distance(0, need["index"]):
					need["item"] = item
					need["index"] = targetindex
	return need
def		farm_ressources(bernard):
	#looking for the needed loots
	find = False
	#list des loot nécessaire pour passer au lvl superieur
	needs_to_incant = incant_need(bernard.lvl, bernard.inventory)
	#list des loot nécessaire pour passer du lvl actuel au lvl max
	needs_total = incant_total(bernard.lvl, bernard.inventory)
	#WIP créer une fonction qui loot une liste d'objet
	#list la pos, le nb et le nom des loots needed dans le champs de vision
	#process un goto sur chaque pos en actualisant la pos de benard dans un tmp
	#pb: list de commands trop longue ?
	#WIP
	need = find_closest_need(bernard, needs_to_incant)
	if need["item"] is not None:
		find = True
		print("going for {} at {}".format(need["item"], need["index"]))
		goto_index(bernard, need["index"])
		farm_case(bernard, needs_to_incant, needs_total, need)
	else:
		print("nothing found in view range")
	return find

def		drop_requirement(bernard):
	print("preparing the ritual...")
	#incant_put return un array contenant les ressources à poser afin d'incanter
	needs_to_put = incant_put(bernard)
	for item in needs_to_put:
		if "player" in item:
			continue
		if needs_to_put[item] > 0:
			print("placing {} {}".format(needs_to_put[item], item))
			compute_action(bernard, C.POSE, needs_to_put[item], item)

class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True or bernard.lvl > 8:
			return
		print("I want to Eleve myself \o/ !")
		if incant_possible(bernard.lvl, bernard.inventory) == False:
			print("incantation is not possible yet !")
			if farm_ressources(bernard) == False:
				print("going forward !")
				compute_action(bernard, C.AVANCE, 2)
				compute_action(bernard, C.DROITE)
				print("scooting the area")
				compute_action(bernard, C.VOIR)
		else:
			print("incantation is possible !")
			drop_requirement(bernard)
			print("I'm Elevating \o/ !")
			# compute_action(bernard, C.INCANTATION)
			bernard.lvl += 1
			print("------------------------------ LVL [ {} ]".format(bernard.lvl))

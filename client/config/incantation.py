from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_find
from action.move import goto_index
from action.incant import incant_possible, incant_need, incant_put, incant_total


def		farm_case(bernard, n, nt, ti, loot):
	view = bernard.view

	print("looting {} {}".format(view[ti][loot], loot))
	compute_action(bernard, C.PREND, bernard.view[ti][loot], loot)
	#on a ramassé une ressource donc on actualise la liste des needs
	n[loot] -= view[ti][loot]
	nt[loot] -= view[ti][loot]
	print("looking for other requirement")
	for element in nt:
		if "player" in element or element == loot:
			continue
		if nt[element] > 0:
			if element in view[ti] and view[ti][element] > 0:
				print("looting {} {}".format(view[ti][element], element))
				compute_action(bernard, C.PREND, view[ti][element], element)
				#on actualise la liste des needs totaux
				nt[element] -= view[ti][element]
	#on ramasse un peu de nourriture si il y en a
	if "nourriture" in view[ti] and view[ti]["nourriture"] > 0:
		print("looting {} nourriture".format(view[ti]["nourriture"]))
		compute_action(bernard, C.PREND, view[ti]["nourriture"], "nourriture")

def		farm_ressources(bernard):
	#looking for the needed loots
	find = False
	#list des loot nécessaire pour passer au lvl superieur
	needs_to_incant = incant_need(bernard.lvl, bernard.inventory)
	#list des loot nécessaire pour passer du lvl actuel au lvl max
	needs_total = incant_total(bernard.lvl, bernard.inventory)
	#WIP créer une fonction qui loot une liste d'objet
	for loot in needs_to_incant:
		if "player" in loot:
			continue
		if needs_to_incant[loot] > 0:
			print("looking for {}".format(loot))
			#cherche la case la plus proche contenant loot
			targetindex = view_find(bernard, bernard.view, loot)
			if targetindex is not None:
				find = True
				print("{} {} ! going to pos: {}".format(bernard.view[targetindex][loot], loot, targetindex))
				goto_index(bernard, targetindex)
				farm_case(bernard, needs_to_incant, needs_total, targetindex, loot)
				break
			else:
				print("not found in view range")
	return find

def		drop_requirement(bernard):
	print("preparing the ritual...")
	#incant_put return un array contenant les ressources à poser afin d'incanter
	needs_to_put = incant_put(bernard)
	for loot in needs_to_put:
		if "player" in loot:
			continue
		if needs_to_put[loot] > 0:
			print("placing {} {}".format(needs_to_put[loot], loot))
			compute_action(bernard, C.POSE, needs_to_put[loot], loot)

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
				print("nothing useful nearby. going forward !'")
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

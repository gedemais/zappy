from utils.command import C
from action.callback import compute_action, blind
from action.view import view_pos, view_find
from action.move import goto_pos
from action.incant import incant_possible, incant_need, incant_put


class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if blind(bernard) == True:
			return
		print("I want to Eleve myself !")
		if incant_possible(bernard.lvl, bernard.inventory) == False:
			print("Incantation is not possible yet")
			#looking for the needed loots
			find = False
			#WIP créer une fonction qui loot une liste d'objet
			#WIP actualiser n quand on ramasse d'autre ressource que l'it loot
			n = incant_need(bernard.lvl, bernard.inventory)
			for loot in n:
				if n[loot] > 0:
					print("looking for {}".format(loot))
					targetindex = view_find(bernard, loot)
					if targetindex is not None:
						find = True
						print("I see {} {} ! going to pos: {}".format(bernard.view[targetindex][loot], loot, targetindex))
						targetx, targety = view_pos(targetindex)
						#WIP verifier si ya d'autre loot utile sur la case
						goto_pos(bernard, targetx, targety)
						print("looting {} {}".format(bernard.view[targetindex][loot], loot))
						compute_action(bernard, C.PREND, bernard.view[targetindex][loot], loot)
			if find == False:
				print("there is nothing useful nearby. I'm going forward !'")
				compute_action(bernard, C.AVANCE, bernard.lvl + 1)
				compute_action(bernard, C.DROITE)
				compute_action(bernard, C.VOIR)	
		else:
			print("Incantation is possible ! preparing the ritual...")
			n = incant_put(bernard)
			for loot in n:
				if n[loot] > 0:
					print("placing {} {}".format(n[loot], loot))
					compute_action(bernard, C.POSE, n[loot], loot)
			print("I'm starting to Elevate \o/ !")
			compute_action(bernard, C.INCANTATION)

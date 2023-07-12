import copy

from utils.command import C
from action.callback import compute_action, is_blind
from action.view import view_pos, view_find
from action.move import goto_pos
from action.incant import incant_possible, incant_need, incant_put, incant_total


class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		if is_blind(bernard) == True:
			return
		print("I want to Eleve myself !")
		if incant_possible(bernard.lvl, bernard.inventory) == False:
			print("Incantation is not possible yet")
			#copying bernard.view
			view = copy.deepcopy(bernard.view)
			#looking for the needed loots
			find = False
			#list des loot nécessaire pour passer au lvl superieur
			n = incant_need(bernard.lvl, bernard.inventory)
			print("need to up", n)
			#list des loot nécessaire pour passer du lvl actuel au lvl max
			nt = incant_total(bernard.lvl, bernard.inventory)
			#WIP créer une fonction qui loot une liste d'objet
			for loot in n:
				if "player" in loot:
					continue
				if n[loot] > 0:
					print("looking for {}".format(loot))
					#cherche la case la plus proche contenant loot
					targetindex = view_find(view, bernard.lvl, bernard.x, bernard.y, loot)
					if targetindex is not None:
						find = True
						print("I see {} {} ! going to pos: {}".format(view[targetindex][loot], loot, targetindex))
						targetx, targety = view_pos(targetindex)
						goto_pos(bernard, targetx, targety)
						print("looting {} {}".format(view[targetindex][loot], loot))
						compute_action(bernard, C.PREND, view[targetindex][loot], loot)
						#on a ramassé une ressource donc on actualise la liste des needs
						n[loot] -= view[targetindex][loot]
						nt[loot] -= view[targetindex][loot]
						#il faut actualiser la case view
						view[targetindex][loot] -= view[targetindex][loot]
						print("I'm gonna check if there is any other useful stuff on this case !")
						for l in nt:
							if "player" in l:
								continue
							if nt[l] > 0:
								if l in view[targetindex] and view[targetindex][l] > 0:
									print("taking {} {}".format(view[targetindex][l], l))
									compute_action(bernard, C.PREND, view[targetindex][l], l)
									#on actualise la liste des needs totaux
									nt[l] -= view[targetindex][l]
									#et la case view
									view[targetindex][l] -= view[targetindex][l]
						break
					else:
						print("not found in view range")
			if find == False:
				print("there is nothing useful nearby. I'm going forward !'")
				compute_action(bernard, C.AVANCE, 2)
				compute_action(bernard, C.DROITE)
			print("scooting the area")
			compute_action(bernard, C.VOIR)
		else:
			print("Incantation is possible ! preparing the ritual...")
			n = incant_put(bernard)
			for loot in n:
				if "player" in loot:
					continue
				if n[loot] > 0:
					print("placing {} {}".format(n[loot], loot))
					compute_action(bernard, C.POSE, n[loot], loot)
			print("I'm starting to Elevate \o/ !")
			# compute_action(bernard, C.INCANTATION)
			bernard.lvl += 1
			print("bernard lvl", bernard.lvl)

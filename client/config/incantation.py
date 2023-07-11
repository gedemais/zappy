from utils.command import C
from action.callback import compute_action
from action.view import view_pos, view_find
from action.move import goto_pos
from action.incant import incant_possible, incant_need, incant_put


class	Incantation:
	def	__init__(self):
		pass

	def	run(bernard):
		print("I want to eleve myself !")
		if incant_possible(bernard.lvl, bernard.inventory) == False:
			print("incantation is not possible yet")
			#looking for the needed loots
			find = False
			n = incant_need(bernard.lvl, bernard.inventory)
			for loot in n:
				if n[loot] > 0:
					print("looking for {}".format(loot))
					targetindex = view_find(bernard, loot)
					if targetindex is not None:
						find = True
						print("I see {} {} ! going to pos: {}".format(bernard.view[targetindex][loot], loot, targetindex))
						targetx, targety = view_pos(targetindex)
						goto_pos(bernard, targetx, targety)
						print("looting {} {}".format(bernard.view[targetindex][loot], loot))
						compute_action(bernard, C.PREND, bernard.view[targetindex][loot], loot)
			if find == False:
				print("there is nothing useful nearby. I'm going forward !'")
				compute_action(bernard, C.AVANCE, bernard.lvl + 1)
				compute_action(bernard, C.DROITE)
				compute_action(bernard, C.VOIR)
					
		else:
			print("incantation is possible ! putting stuff on the ground")
			n = incant_put(bernard)
			for loot in n:
				if n[loot] > 0:
					print("dropping {} {}".format(n[loot], loot))
					compute_action(bernard, C.POSE, n[loot], loot)
			print("I'm starting to elevate !!")
			compute_action(bernard, C.INCANTATION)

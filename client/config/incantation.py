from utils.command import C
from action.callback import compute_action
from action.view import view_pos, view_find
from action.move import goto_pos
from action.incant import incant_possible, incant_need, incant_put


class	Incantation:
	def	__init__(self):
		pass

	def	run(self, bernard):
		#check prerequis pour lvl up
		#ramasser si besoin
		#si tout est ok on pose tout et on incant
		n = incant_need(bernard.lvl, bernard.inventory)
		if incant_possible(bernard.lvl, bernard.inventory) == False:
			print("incant not possible yet ! going to loot")
			for loot in n:
				if n[loot] > 0:
					targetindex = view_find(bernard, loot)
					if targetindex is not None:
						print("see {} ! going to pos: {}".format(loot, targetindex))
						targetx, targety = view_pos(targetindex)
						goto_pos(bernard, targetx, targety)
						compute_action(bernard, C.PREND, bernard.view[targetindex][loot], loot)
						return
			print("no loot needed in view ! moving")
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
			compute_action(bernard, C.DROITE)
		else:
			print("incant is possible ! putting stuff on the ground")
			n = incant_put(bernard)
			for loot in n:
				if n[loot] > 0:
					print("putting {} {} on the ground !".format(n[loot], loot))
					compute_action(bernard, C.POSE, n[loot], loot)

from enum import Enum

from utils.command import C, S, Command
from action.action import compute_action
from action.view import view_pos, outofview, view_find
from action.move import goto_pos
from action.incant import incant_possible, incant_need, incant_put


class	T(Enum):
	NONE = 0
	MANGER = 1
	INCANTATION = 2

class	Rush:
	tasks = {
		T.MANGER		: Command(id = T.MANGER),
		T.INCANTATION	: Command(id = T.INCANTATION),
	}

	def	__init__(self):
		pass

	#WIP
	def	task_assign(self, bernard):
		#on reset les tasks en cours
		for task in self.tasks:
			self.tasks[task].state = S.NONE
		if bernard.inventory != None and len(bernard.inventory) > 0 and bernard.inventory["nourriture"] < 5:
			self.tasks[T.MANGER].state = S.NEED
			return
		#si le bot ne meurs pas de fin on va tenter une incantation
		self.tasks[T.INCANTATION].state = S.NEED

	#assigne une tache au joueur celon ses besoins
	def	task_manager(self, bernard):
		#WIP
		self.task_assign(bernard)
		#WIP
		if self.tasks[T.MANGER].state == S.NEED:
			#il faut trouver de la nourriture
			print("T.MANGER")
			#y a t-il de la nourriture proche ?
			targetindex = view_find(bernard, "nourriture")
			if targetindex is not None:
				loot = bernard.view[targetindex]
				if "nourriture" in loot and loot["nourriture"] > 0:
					#oui : y aller et prendre
					print("see nourriture ! going to pos: {}".format(targetindex))
					targetx, targety = view_pos(targetindex)
					goto_pos(bernard, targetx, targety)
					compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
					return
			else:
				#non : avancer
				print("no food in vision")
				compute_action(bernard, C.AVANCE, bernard.lvl + 1)
				compute_action(bernard, C.DROITE, 1)
		if self.tasks[T.INCANTATION].state == S.NEED:
			#WIP
			if bernard.inventory == None or len(bernard.inventory) == 0:
				return
			print("T.INCANTATION")
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

	#celon les données de bernard on assigne de nouvelles taches
	def	run(self, bernard):
		print("road to level2 ! =================")
		#first view or update view
		if bernard.view == None or len(bernard.view) == 0 \
				or outofview(bernard.x, bernard.y, bernard.lvl) == True:
			compute_action(bernard, C.VOIR)
			return
		#first inventory or update inventory (each 2s)
		if bernard.inventory == None or len(bernard.inventory) == 0 \
				or bernard.t - bernard.update_inventory > 2000:
			compute_action(bernard, C.INVENTAIRE)
		#task manager
		self.task_manager(bernard)
		print("==================================")

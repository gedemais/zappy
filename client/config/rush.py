from enum import Enum

from utils.command import C, S, Command
from action.action import compute_action
from action.view import getviewindex, outofview, getviewpos
from action.move import goto_pos


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
		if bernard.inventory != None and len(bernard.inventory) > 0 and bernard.inventory["nourriture"] < 50:
			self.tasks[T.MANGER].state = S.NEEDED
			return
		#si le bot ne meurs pas de fin on va tenter une incantation
		self.tasks[T.INCANTATION].state = S.NEEDED

	#assigne une tache au joueur celon ses besoins
	def	task_manager(self, bernard):
		self.task_assign(bernard)
		if self.tasks[T.MANGER].state == S.NEEDED:
			#il faut trouver de la nourriture
			print("T.MANGER")
			#y a t-il de la nourriture proche ?
			#oui : y aller et prendre
			for i in range(len(bernard.view)):
				loot = bernard.view[i]
				if "nourriture" in loot and loot["nourriture"] > 0:
					print("see food ! going to pos: {}".format(i))
					print(loot)
					targetx, targety = getviewpos(i)
					goto_pos(bernard, targetx, targety)
					compute_action(bernard, C.PREND, loot["nourriture"], "nourriture")
					return
			#non : avancer
			print("no food in vision")
			compute_action(bernard, C.AVANCE, bernard.lvl + 1)
		if self.tasks[T.INCANTATION].state == S.NEEDED:
			#WIP
			print("T.INCANTATION")

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

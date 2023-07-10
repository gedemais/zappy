from enum import Enum

from command import C, S, Command
from action import compute_action, getviewindex, outofview, getviewpos
from random import randint


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
		if bernard.inventory != None and len(bernard.inventory) > 0 and bernard.inventory["nourriture"] < 10:
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
			#y a t-il de la nourriture ou je suis ?
			#oui : prendre
			index = getviewindex(bernard.x, bernard.y)
			print(bernard.view[index])
			if "nourriture" in bernard.view[index] and bernard.view[index]["nourriture"] > 0:
				print("food on bot pos, on prend")
				compute_action(bernard.needs[C.PREND], bernard.view[index]["nourriture"], ["nourriture"])
			#non : avancer
			else:
				print("no food on bot pos, on avance")
				compute_action(bernard.needs[C.AVANCE])
		if self.tasks[T.INCANTATION].state == S.NEEDED:
			#WIP
			print("T.INCANTATION")

	#celon les données de bernard on assigne de nouvelles taches
	def	run(self, bernard):
		print("road to level2 ! =================")
		#first view or update view
		if bernard.view == None or len(bernard.view) == 0 \
				or outofview(bernard.x, bernard.y, bernard.lvl) == True:
			compute_action(bernard.needs[C.VOIR])
			# compute_action(bernard.needs[C.DROITE if randint(0, 100) < 50 else C.GAUCHE])
			return
		#first inventory or update inventory (each 2s)
		if bernard.inventory == None or len(bernard.inventory) == 0 \
				or bernard.t - bernard.update_inventory > 2000:
			compute_action(bernard.needs[C.INVENTAIRE])
		#task manager
		self.task_manager(bernard)
		print("==================================")

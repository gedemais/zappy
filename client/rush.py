from enum import Enum

from command import C, S, Command
from action import getviewindex, outofview
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
	ticks = 0

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
			#suis-je en dehors de mon champ de vision ?
			if outofview(bernard.x, bernard.y, bernard.lvl) == True:
				bernard.needs[C.VOIR].update(state = S.NEEDED)
				bernard.needs[C.DROITE if randint(0, 100) < 50 else C.GAUCHE].update(state = S.NEEDED)
				return
			#y a t-il de la nourriture ou je suis ?
			#oui : prendre
			index = getviewindex(bernard.x, bernard.y)
			print(bernard.view[index])
			if "nourriture" in bernard.view[index] and bernard.view[index]["nourriture"] > 0:
				print("food on bot pos, on prend")
				bernard.needs[C.PREND].update(state = S.NEEDED, buf = "nourriture")
			#non : avancer
			else:
				print("no food on bot pos, on avance")
				bernard.needs[C.AVANCE].update(state = S.NEEDED)
		if self.tasks[T.INCANTATION].state == S.NEEDED:
			print("T.INCANTATION")

	#celon les données de bernard on assigne de nouvelles taches
	def	run(self, bernard):
		self.ticks = self.ticks + 1
		print("road to level2 ! =================", bernard.ticks, self.ticks)
		#first view
		if bernard.view == None or len(bernard.view) == 0:
			#WIP
			bernard.t = bernard.ticks
			###
			bernard.needs[C.VOIR].update(state = S.NEEDED)
			return
		#first inventory
		if bernard.inventory == None or len(bernard.inventory) == 0:
			bernard.needs[C.INVENTAIRE].update(state = S.NEEDED)
		#update inventory
		print("ticks", bernard.ticks, "st", self.ticks, "te", bernard.te, bernard.te * 4)
		if bernard.ticks - self.ticks > bernard.te * 4:
			bernard.needs[C.INVENTAIRE].update(state = S.NEEDED)
			self.ticks = bernard.ticks
		#WIP
		self.task_manager(bernard)
		print("==================================")

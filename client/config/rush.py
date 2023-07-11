from enum import Enum

from utils.command import C, S, Command
from action.callback import compute_action
from action.view import outofview
from config.manger import Manger
from config.incantation import Incantation


class	T(Enum):
	NONE = 0
	MANGER = 1
	INCANTATION = 2
	BROADCAST = 3

tasks = {
	T.MANGER		: Command(id = T.MANGER),
	T.INCANTATION	: Command(id = T.INCANTATION),
	T.BROADCAST		: Command(id = T.BROADCAST),
}

#assigne une tache au joueur celon ses besoins
def		task_manager(bernard):
	#WIP
	if tasks[T.MANGER].state == S.NEED:
		#il faut trouver de la nourriture
		print("T.MANGER")
		Manger.run(bernard)
		return
	if tasks[T.INCANTATION].state == S.NEED:
		#WIP
		print("T.INCANTATION")
		Incantation.run(bernard)
	if tasks[T.BROADCAST].state == S.NEED:
		#WIP
		print("T.BROADCAST")

#WIP
def		task_assign(bernard):
	#il faut de la nourriture
	if bernard.inventory["nourriture"] < 5:
		tasks[T.MANGER].state = S.NEED
	elif bernard.inventory["nourriture"] > 10:
		tasks[T.MANGER].state = S.NONE
	#si le bot ne meurs pas de fin on va tenter une incantation
	tasks[T.INCANTATION].state = S.NEED

class	Rush:
	def	__init__(self):
		pass

	#celon les données de bernard on assigne de nouvelles taches
	def	run(bernard):
		print("road to level2 ! =================")
		if bernard.view is not None and bernard.inventory is not None \
				and len(bernard.view) > 0 and len(bernard.inventory) > 0:
			#update view if out of view array
			if outofview(bernard.x, bernard.y, bernard.lvl) == True:
				compute_action(bernard, C.VOIR)
			#update inventory (each 2s)
			if bernard.t - bernard.update_inventory > 2000:
				compute_action(bernard, C.INVENTAIRE)
			#WIP
			task_assign(bernard)
			#WIP
			task_manager(bernard)
		else:
			compute_action(bernard, C.VOIR)
			compute_action(bernard, C.INVENTAIRE)
		print("==================================")

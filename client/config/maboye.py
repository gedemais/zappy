from enum import Enum

from utils.command import S, Command
from action.callback import is_blind
from action.incant import incant_total
from config.collect import Collect
from config.manger import Manger
from config.meet import Meet
from config.rush import Rush
from action.view import view_index, view_pos


class	T(Enum):
	NONE = 0
	MANGER = 1
	RUSH = 2
	COLLECT = 3
	MEET = 4

tasks = {
	T.MANGER		: Command(id = T.MANGER),
	T.RUSH			: Command(id = T.RUSH),
	T.COLLECT		: Command(id = T.COLLECT),
	T.MEET			: Command(id = T.MEET),
}

#assigne une tache au joueur celon ses besoins
def		task_manager(bernard):
	#WIP
	if tasks[T.MANGER].state == S.NEED:
		#il faut trouver de la nourriture
		print("T.MANGER")
		Manger.run(bernard)
		return
	if tasks[T.RUSH].state == S.NEED:
		#rush le lvl demandé (2 puis 8)
		print("T.RUSH")
		Rush.run(bernard, bernard.rushlvl)
		return
	if tasks[T.COLLECT].state == S.NEED:
		#on collect tout ce qu'il manque pour up lvl 8
		print("T.COLLECT")
		Collect.run(bernard, incant_total(bernard, 8))
		return
	if tasks[T.MEET].state == S.NEED:
		#on rejoins les autres joueurs de la team
		print("T.MEET")
		Meet.run(bernard)
		return

#WIP
def		task_assign(bernard):
	#il faut de la nourriture
	if bernard.inventory["nourriture"] < 5:
		tasks[T.MANGER].state = S.NEED
		return
	elif bernard.inventory["nourriture"] > 10:
		tasks[T.MANGER].state = S.NONE
	# rush lvl 2
	if bernard.lvl < 2:
		tasks[T.RUSH].state = S.NEED
		bernard.rushlvl = 2
		return
	else:
		tasks[T.RUSH].state = S.NONE

	#on verifie si il manque des ressources pour passer lvl 8
	miss = False
	it = incant_total(bernard, 8)
	for item in it:
		if "player" not in item and it[item] > 0:
			miss = True
	#si il manque des ressources alors on va les collect
	if miss == True:
		tasks[T.COLLECT].state = S.NEED
		return
	else:
		tasks[T.COLLECT].state = S.NONE
	#WIP quand la collecte sera terminée on est lvl 2 et on a de quoi up lvl 8
	#il faut rejoindre les autres joueurs (manger sur le trajet)
	tasks[T.MEET].state = S.NEED
	return
	#une fois que les 6 joueurs sont sur la même case on les fait tous up du lvl 2 à 8
	tasks[T.RUSH].state = S.NEED
	bernard.rushlvl = 8

class	Maboye:
	def	__init__(self):
		pass

	#celon les données de bernard on assigne de nouvelles taches
	def	run(bernard):
		print("road to level 8 ! ================")
		if is_blind(bernard) == True:
			return
		print("[ bernard ] - lvl: {} - food: {} - pos: {}, {} - index: {} - dir: {} - vpos: {}".format(\
			bernard.lvl,\
			bernard.inventory["nourriture"],\
			bernard.x, bernard.y,\
			view_index(bernard.dir, bernard.x, bernard.y),\
			bernard.dir,\
			view_pos(bernard.dir, view_index(bernard.dir, bernard.x, bernard.y)),\
		))
		#WIP
		task_assign(bernard)
		#WIP
		task_manager(bernard)
		print("==================================")

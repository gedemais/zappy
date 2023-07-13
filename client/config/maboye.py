from enum import Enum

from utils.command import S, Command
from action.callback import is_blind
from action.incant import incant_total
from config.collect import Collect
from config.manger import Manger
from config.rush import Rush


class	T(Enum):
	NONE = 0
	MANGER = 1
	RUSH = 2
	COLLECT = 3
	MEET = 4
	INCANTATION = 5

tasks = {
	T.MANGER		: Command(id = T.MANGER),
	T.RUSH			: Command(id = T.RUSH),
	T.COLLECT		: Command(id = T.COLLECT),
	T.MEET			: Command(id = T.MEET),
	T.INCANTATION	: Command(id = T.INCANTATION),
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
		#rush lvl 2
		print("T.RUSH")
		Rush.run(bernard, 2)
		return
	if tasks[T.COLLECT].state == S.NEED:
		#on collect tout ce qu'il manque pour up lvl 8
		print("T.COLLECT")
		Collect.run(bernard, incant_total(bernard.inventory, bernard.lvl, 8))
		return

#WIP
def		task_assign(bernard):
	#il faut de la nourriture
	if bernard.inventory["nourriture"] < 5:
		tasks[T.MANGER].state = S.NEED
	elif bernard.inventory["nourriture"] > 10:
		tasks[T.MANGER].state = S.NONE
	#rush lvl 2
	if bernard.lvl < 2:
		tasks[T.RUSH].state = S.NEED
	#on ramasse des ressources
	else:
		#WIP add une variable qui simule la prise de lvl pour stop la collect
		# ou verifier si incant_total dans la task Collect est vide, si oui on change de task
		tasks[T.RUSH].state = S.NONE
		tasks[T.COLLECT].state = S.NEED
	#WIP quand la collecte sera terminée on est lvl 2 et on a de quoi up lvl 8
	#il faut rejoindre les autres joueurs (manger sur le trajet)
	#une fois que les 6 joueurs sont sur la même case on les fait tous up du lvl 2 à 8

class	Maboye:
	def	__init__(self):
		pass

	#celon les données de bernard on assigne de nouvelles taches
	def	run(bernard):
		print("road to level 8 ! ================")
		if is_blind(bernard) == True:
			return
		print("bernard lvl {} food {}".format(bernard.lvl, bernard.inventory["nourriture"]))
		#WIP
		task_assign(bernard)
		#WIP
		task_manager(bernard)
		print("==================================")

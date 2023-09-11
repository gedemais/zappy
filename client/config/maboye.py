from enum import Enum

from utils.command import C, S, Command
from action.utils import is_blind, compute_action, send_broadcast
from action.incant import incant_total
from config.collect import Collect
from config.manger import Manger
from config.meet import Meet
from config.rush import Rush
from config.hatch import Hatch
from action.view import view_index


class	T(Enum):
	NONE = 0
	MANGER = 1
	RUSH = 2
	COLLECT = 3
	MEET = 4
	HATCH = 5

tasks = {
	T.MANGER		: Command(id = T.MANGER),
	T.RUSH			: Command(id = T.RUSH),
	T.COLLECT		: Command(id = T.COLLECT),
	T.MEET			: Command(id = T.MEET),
	T.HATCH			: Command(id = T.HATCH),
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
	if tasks[T.HATCH].state == S.NEED:
		#on recrute le max de joueurs
		print("T.HATCH")
		Hatch.run(bernard)
		return
	if tasks[T.COLLECT].state == S.NEED:
		#on collect tout ce qu'il manque pour up lvl 8
		print("T.COLLECT")
		Collect.run(bernard, incant_total(bernard, bernard.rushlvl))
		return
	if tasks[T.MEET].state == S.NEED:
		#on rejoins les autres joueurs de la team
		print("T.MEET")
		Meet.run(bernard)
		return

#WIP
def		task_assign(bernard):
	#il faut de la nourriture
	if bernard.inventory["nourriture"] < bernard.foodmin:
		tasks[T.MANGER].state = S.NEED
		return
	elif bernard.inventory["nourriture"] >= bernard.foodmax:
		tasks[T.MANGER].state = S.NONE
	print("player on my case: {}".format(bernard.view[0]))
	# rush lvl 2
	if bernard.lvl < 2:
		tasks[T.RUSH].state = S.NEED
		return
	else:
		tasks[T.RUSH].state = S.NONE
	# rush lvl 8
	if bernard.rushfinal == True:
		bernardindex = view_index(bernard.x, bernard.y)
		if "player" in bernard.view[bernardindex] and bernard.view[bernardindex]["player"] == 6:
			tasks[T.RUSH].state = S.NEED
			return
		else:
			compute_action(bernard, C.VOIR, 1)
			tasks[T.RUSH].state = S.NONE
			bernard.rushfinal = False
	# on recrute le max de joueur possible
	if bernard.team_total < 6\
			and (bernard.last_hatch == 0 or bernard.t - bernard.last_hatch > 20000):
		tasks[T.HATCH].state = S.NEED
		return
	else:
		tasks[T.HATCH].state = S.NONE
	if bernard.team_total < 6:
		bernard.foodmin = 20
		bernard.foodmax = 25
		return
	#on verifie si il manque des ressources pour passer lvl 8
	bernard.rushlvl = 8
	miss = False
	it = incant_total(bernard, bernard.rushlvl)
	for item in it:
		if "player" not in item and it[item] > 0:
			miss = True
	#si il manque des ressources alors on va les collect
	#on prevoit de la bouffe
	if miss == True:
		bernard.foodmin = 20
		bernard.foodmax = 25
		tasks[T.COLLECT].state = S.NEED
		return
	else:
		tasks[T.COLLECT].state = S.NONE
	if bernard.leader is None:
		bernard.foodmin = 40
		bernard.foodmax = 50
	else:
		bernard.foodmin = 10
		bernard.foodmax = 25
	#quand la collecte est terminée on est lvl 2 et on a de quoi up lvl 8, manger
	if bernard.inventory["nourriture"] < bernard.foodmin:
		tasks[T.MANGER].state = S.NEED
		return
	#il faut rejoindre les autres joueurs
	if "player" in bernard.view[0] and bernard.view[0]["player"] < 6:
		print("player on my case: {}".format(bernard.view[0]))
		tasks[T.MEET].state = S.NEED
		return
	else:
		tasks[T.MEET].state = S.NONE
	#une fois que les 6 joueurs sont sur la même case on les fait tous up du lvl 2 à 8
	bernard.rushfinal = True

class	Maboye:
	def	__init__(self):
		pass

	#celon les données de bernard on assigne de nouvelles taches
	def	run(bernard):
		print("road to level 8 ! ================")
		if is_blind(bernard) == True:
			return
		# for elt in bernard.view:
		# 	print(elt)
		print("[ bernard ] - lvl: {} - food: {} - leader: {} - fmin : {} - fmax {}".format(\
			bernard.lvl,\
			bernard.inventory["nourriture"],\
			bernard.leader,\
			bernard.foodmin,\
			bernard.foodmax,\
		))
		if bernard.hatched == True:
			send_broadcast(bernard, "I just hatched an egg !")
			bernard.hatched = False
		#WIP
		task_assign(bernard)
		#WIP
		task_manager(bernard)
		print("==================================")

from enum import Enum

from utils.command import C, S, Command
from action.utils import is_blind, compute_action, send_broadcast
from action.incant import incant_total, incant_possible
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
		Manger.run(bernard)
		return
	if tasks[T.RUSH].state == S.NEED:
		#rush le lvl demandé (2 puis 8)
		Rush.run(bernard, bernard.rushlvl)
		return
	if tasks[T.HATCH].state == S.NEED:
		#on recrute le max de joueurs
		Hatch.run(bernard)
		return
	if tasks[T.COLLECT].state == S.NEED:
		#on collect tout ce qu'il manque pour up lvl 8
		Collect.run(bernard, incant_total(bernard, bernard.rushlvl))
		return
	if tasks[T.MEET].state == S.NEED:
		#on rejoins les autres joueurs de la team
		Meet.run(bernard)
		return

#WIP
def		task_assign(bernard):
	bernardindex = view_index(bernard.x, bernard.y)
	print(bernard.view[bernardindex])
	#il faut de la nourriture
	if bernard.inventory["nourriture"] < bernard.foodmin:
		tasks[T.MANGER].state = S.NEED
		return
	elif bernard.inventory["nourriture"] >= bernard.foodmax:
		tasks[T.MANGER].state = S.NONE
	# rush lvl 2
	if bernard.lvl < 2:
		tasks[T.RUSH].state = S.NEED
		return
	else:
		tasks[T.RUSH].state = S.NONE
	# rush lvl 8
	if bernard.rushfinal == True:
		print("rush final")
		#on attend 2secondes pour verifier si il ya toujours 6 joueurs sur la case
		r = 2000
		if "nourriture" in bernard.view and bernard.view["nourriture"] > 0:
			r = bernard.view["nourriture"] * 100
		if bernard.t - bernard.rushtime < r:
			if bernard.leader == -1:
				send_broadcast(bernard, "Come to your leader !")
			print("waiting validation")
			return
		#si oui on envoit les 6 incantations
		elif incant_possible(bernard, False) == True\
				and "player" in bernard.view[bernardindex] and bernard.view[bernardindex]["player"] == 6:
			bernard.foodmin = 1
			bernard.foodmax = 10
			tasks[T.RUSH].state = S.NEED
			return
		#si non on reprend la phase de meet
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
	#en attendant d'etre 6 on collect de la nourriture
	if bernard.team_total < 6:
		bernard.foodmin = bernard.inventory["nourriture"] + 1
		bernard.foodmax = bernard.foodmin
		return
	#on verifie si il manque des ressources pour passer lvl 8
	bernard.rushlvl = 8
	miss = False
	it = incant_total(bernard, bernard.rushlvl)
	for item in it:
		if "player" not in item and it[item] > 0:
			miss = True
	#si il manque des ressources alors on va les collect
	#et on prevoit un minimum de bouffe
	if miss == True:
		bernard.foodmin = bernard.wr / 2
		bernard.foodmax = bernard.foodmin + 5
		bernard.rushfinal = False
		tasks[T.COLLECT].state = S.NEED
		return
	else:
		tasks[T.COLLECT].state = S.NONE
	#les premiers bots collectent plus de nourritures que les suivant
	#quand un leader est set ils le rejoignent avec un seuil de nourriture faible
	if bernard.leader is None:
		bernard.foodmin = bernard.wr * 2
		bernard.foodmax = bernard.foodmin + 10
	else:
		bernard.foodmin = bernard.wr / 2
		bernard.foodmax = bernard.foodmin + 5
	if bernard.inventory["nourriture"] < bernard.foodmin:
		tasks[T.MANGER].state = S.NEED
		return
	#quand la collecte est terminée on est lvl 2 et on a de quoi up lvl 8
	#il faut rejoindre les autres joueurs
	if "player" not in bernard.view[bernardindex] or bernard.view[bernardindex]["player"] < 6:
		tasks[T.MEET].state = S.NEED
		return
	else:
		tasks[T.MEET].state = S.NONE
	#une fois que les 6 joueurs sont sur la même case on les fait tous up du lvl 2 à 8
	bernard.rushfinal = True
	bernard.rushtime = bernard.t

class	Maboye:
	def	__init__(self):
		pass

	#celon les données de bernard on assigne de nouvelles taches
	def	run(bernard):
		if is_blind(bernard) == True:
			return
		print("================================== [ bernard {} ] [ lvl {} - food {} ]".format(\
			bernard.id,\
			bernard.lvl,\
			bernard.inventory["nourriture"],\
		))
		print("leader: {} - tt: {} - fmin: {} - fmax: {}".format(\
			bernard.leader,\
			bernard.team_total,\
			bernard.foodmin,\
			bernard.foodmax,\
		))
		if bernard.hatched == True:
			send_broadcast(bernard, "I just hatched an egg !")
			bernard.hatched = False
		if bernard.leader != -1 and bernard.t - bernard.leader_contact > 5000:
			bernard.leader = None
		task_assign(bernard)
		task_manager(bernard)
		print("==================================")
		print("==================================")

import random

from utils.command import C, Command
from action.view import outofview


# append l'action demandée dans une queue de Command
# repeat	: le nombre de fois ou elle sera executée
# element	: string ou array lié(e) à la commande
#	pour chaque element de l'array une commande sera executée avec buf = array[i]
#	ça permet d'envoyer une list de ressources à poser/prendre
def		compute_action(bernard, id, repeat = 1, element = None):
	if repeat > 5:
		repeat = 5
	for i in range(repeat):
		command = Command(id = id)
		command.buf = element
		bernard.actions.append(command)

def		view_rand(bernard):
	r = random.randint(0, 1)
	if r == 0:
		compute_action(bernard, C.DROITE, 1)
	else:
		compute_action(bernard, C.GAUCHE, 1)

# 1 is front
# 2 1 8
# 3 X 7
# 4 5 6
def		set_dir(bernard, default = True):
	front = 0
	right = 0
	back = 0
	left = 0

	if bernard.leader is not None and bernard.leader != -1 and bernard.leader_meet == True:
		d = bernard.leader
		if d == 1 or d == 2 or d == 8:
			#front
			view_rand(bernard)
		elif d == 7:
			#right
			compute_action(bernard, C.DROITE, 1)
		if d == 5 or d == 4 or d == 6:
			#back
			compute_action(bernard, C.DROITE, 2)
		elif d == 3:
			#left
			compute_action(bernard, C.GAUCHE, 1)
		return


	if len(bernard.bdir) > 5:
		for d in bernard.bdir:
			if d == 1 or d == 2 or d == 8:
				#front
				front = front + 1
			elif d == 7:
				#right
				right = right + 1
			if d == 5 or d == 4 or d == 6:
				#back
				back = back + 1
			elif d == 3:
				#left
				left = left + 1
		bernard.bdir = []
		# 0 : front - 1 : right - 2 : back - 3 : left
		tmp = 0
		if right > front:
			front = right
			tmp = 1
		if back > front:
			front = back
			tmp = 2
		if left > front:
			front = left
			tmp = 3
		if tmp == 0:
			pass
		elif tmp == 1:
			compute_action(bernard, C.DROITE, 1)
		elif tmp == 2:
			compute_action(bernard, C.DROITE, 2)
		elif tmp == 3:
			compute_action(bernard, C.GAUCHE, 1)
	elif default == True:
		view_rand(bernard)

def		is_blind(bernard):
	blind = False

	if bernard.view == None or bernard.inventory == None or bernard.team_slot == None\
			or type(bernard.view) != list or type(bernard.inventory) != dict\
				or len(bernard.view) < 4 or len(bernard.inventory) == 0:
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.INVENTAIRE, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		send_broadcast(bernard, "My level is : {}".format(bernard.lvl))
		blind = True
	#update view if out of view array
	elif outofview(bernard, bernard.x, bernard.y) == True:
		set_dir(bernard, False)
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
	elif bernard.ko == True and bernard.leader is not None:
		view_rand(bernard)
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
	#update inventory (each 5s)
	elif bernard.t - bernard.last_inventory > 5000:
		set_dir(bernard, False)
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.INVENTAIRE, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		if bernard.leader != -1 and bernard.leader is not None:
			send_broadcast(bernard, "My level is : {}".format(bernard.lvl))
		blind = True
	return blind

def		send_broadcast(bernard, message):
	compute_action(bernard, C.BROADCAST, 1, "team_name " + bernard.team_name + ' ' + message)

def		case_value(case, values):
	tmp = 0
	#pour chaque items
	for item in case:
		if "player" not in item and "visited" not in item:
			#on lui attribue un poid qui depend de la quantite dont il a besoin de l'item
			if "nourriture" in item:
				r = 0.5
			elif item in values:
				r = values[item]
			else:
				break
			tmp = tmp + case[item] * r
	return tmp

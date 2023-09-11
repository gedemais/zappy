import random

from utils.command import C, Command
from action.view import outofview


# append l'action demandée dans une queue de Command
# repeat	: le nombre de fois ou elle sera executée
# element	: string ou array lié(e) à la commande
#	pour chaque element de l'array une commande sera executée avec buf = array[i]
#	ça permet d'envoyer une list de ressources à poser/prendre
def		compute_action(bernard, id, repeat = 1, element = None):
	if repeat > 6:
		repeat = 6
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

def		is_blind(bernard):
	blind = False

	if bernard.ko == True and bernard.leader != -1\
			and "player" in bernard.view[0] and bernard.view[0]["player"] == 0:
		print("Someone is around !")
		view_rand(bernard)
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
		return
	if bernard.view == None or bernard.inventory == None or bernard.team_slot == None\
			or len(bernard.view) == 0 or len(bernard.inventory) == 0:
		print("I'm blind :'(")
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.INVENTAIRE, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
	#update view if out of view array
	elif outofview(bernard, bernard.x, bernard.y) == True:
		print("I'm lost in the dark")
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
	#update inventory (each 5s)
	if bernard.t - bernard.last_inventory > 5000:
		print("I need to check my stuff !")
		compute_action(bernard, C.VOIR, 1)
		compute_action(bernard, C.INVENTAIRE, 1)
		compute_action(bernard, C.CONNECT_NBR, 1)
		blind = True
	return blind

#WIP
def	send_broadcast(bernard, message):
	compute_action(bernard, C.BROADCAST, 1, "team_name " + bernard.team_name + ' ' + message)

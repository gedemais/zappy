from utils.command import S, Command
from action.view import view_index


# append l'action demandée dans une queue de Command
# repeat	: le nombre de fois ou elle sera executée
# element	: string ou array lié(e) à la commande
#	pour chaque element de l'array une commande sera executée avec buf = array[i]
#	ça permet d'envoyer une list de ressources à poser/prendre
def		compute_action(bernard, id, repeat = 1, element = None):
	for i in range(repeat):
		command = Command(id = id)
		command.buf = element
		bernard.actions.append(command)

class	Action:
	def	__init__(self):
		pass

	def	voir(bernard, command):
		bernard.view = []
		#on reset la position du joueur
		bernard.x, bernard.y = 0, 0
		for loot in command.response:
			bernard.view.append(loot)
		bernard.view_size = len(bernard.view)
		print("[ voir ]", bernard.view)

	def	inventaire(bernard, command):
		#last inventory update
		bernard.update_inventory = bernard.t
		bernard.inventory = command.response
		print("[ inventaire ]", bernard.inventory)

	def	prend(bernard, command):
		#update view
		viewcase = bernard.view[view_index(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] -= 1
			#update inventory
			for element in bernard.inventory:
				if element == command.buf:
					index = element
					bernard.inventory[element] += 1
			print("[ prend ] - {}".format(command.buf))
			print("inventaire: {} -> {}".format(bernard.inventory[index] - 1, bernard.inventory[index]))
			print("viewcase: index {}, {} -> {}"
				.format(view_index(bernard.x, bernard.y), viewcase[command.buf] + 1, viewcase[command.buf]))

	def	pose(bernard, command):
		#update view
		viewcase = bernard.view[view_index(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] += 1
			#update inventory
			for element in bernard.inventory:
				if element == command.buf:
					index = element
					bernard.inventory[element] -= 1
			print("[ pose ] - {}".format(command.buf))
			print("inventaire: {} -> {}".format(bernard.inventory[index] + 1, bernard.inventory[index]))
			print("viewcase: index {}, {} -> {}"
				.format(view_index(bernard.x, bernard.y), viewcase[command.buf] + 1, viewcase[command.buf]))

	def	droite(bernard, command):
		tmp = bernard.dir
		#update direction by right
		bernard.dir = bernard.dir + 90
		if bernard.dir == 270:
			bernard.dir = -90
		print("[ droite ] - dir: {} -> {}".format(tmp, bernard.dir))

	def	gauche(bernard, command):
		tmp = bernard.dir
		#update direction by left
		bernard.dir = bernard.dir - 90
		if bernard.dir == -270:
			bernard.dir = 90
		print("[ gauche ] - dir: {} -> {}".format(tmp, bernard.dir))

	def	avance(bernard, command):
		#update direction by forward
		#front
		if bernard.dir == 0:
			bernard.y += 1
			bernard.sy += 1
		#back
		if bernard.dir == 180 or bernard.dir == -180:
			bernard.y -= 1
			bernard.sy -= 1
		#right
		if bernard.dir == 90:
			bernard.x += 1
			bernard.sx += 1
		#left
		if bernard.dir == -90:
			bernard.x -= 1
			bernard.sx -= 1
		print("[ avance ] - pos: {}, {} - spos: {}, {} - index: {}"
			.format(bernard.x, bernard.y, bernard.sx, bernard.sy, view_index(bernard.x, bernard.y)))

	def	connect_nbr():
		pass

	def incantation():
		pass

	def	fork():
		pass

	def	expulse():
		pass

	def	broadcast():
		pass

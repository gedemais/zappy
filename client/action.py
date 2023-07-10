from command import S


# need		: la commande need qui doit être compute
# repeat	: le nombre de fois ou elle sera executée
# element	: string ou array lié(e) à la commande
#	pour chaque element de l'array une commande sera executée avec buf = array[i]
#	ça permet d'envoyer une list de ressources à poser/prendre
def	compute_action(need, repeat = 1, element = None):
	need.update(state = S.NEEDED)
	need.buf = None
	need.repeat = repeat
	if element != None:
		need.buf = []
		if type(element) == list:
			for elt in element:
				need.buf.append(elt)
		else:
			need.buf.append(element)

def	goto_index(bernard, index):
	pass

#return the index of bernard in view with x, y
def	getviewindex(x, y):
	index = 0
	middle = x * (x + 1)
	index = middle + y
	return index

#return the pos of bernard in view with index
def	getviewpos(index):
	x, y = 0, 0
	#if index == 0, pox == 0, 0
	if index == 0:
		return x, y
	start, end = 0, 0
	for i in range(1, 8):
		lvl = i + 1
		middle = lvl * i
		start = middle - lvl
		end = middle + lvl
		if index > start and index < end:
			break
	x = lvl - 1
	y = -x
	for i in range(start, end):
		if index == getviewindex(x, y):
			break
		y += 1
	return x, y

#return true if x, y are out of bernard.view
def	outofview(x, y, lvl):
	offset = lvl
	if x < 0 or x > offset:
		return True
	offset = abs(lvl - lvl - x)
	if y < -offset or y > offset:
		return True
	return False

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
		print("voir", bernard.view)

	def	inventaire(bernard, command):
		#last inventory update
		bernard.update_inventory = bernard.t
		bernard.inventory = command.response
		print("inventaire", bernard.inventory)

	def	prend(bernard, command):
		#update inventory
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] += 1
		#update view
		viewcase = bernard.view[getviewindex(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] -= 1
		print("prend", command.buf)

	def	pose(bernard, command):
		#update inventory
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] -= 1
		#update view
		viewcase = bernard.view[getviewindex(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] += 1
		print("pose", command.buf)

	def	droite(bernard, command):
		#update direction by right
		bernard.dir = bernard.dir + 90
		if bernard.dir == 270:
			bernard.dir = -90
		print("droite", bernard.dir)

	def	gauche(bernard, command):
		#update direction by left
		bernard.dir = bernard.dir - 90
		if bernard.dir == -270:
			bernard.dir = 90
		print("gauche", bernard.dir)

	def	avance(bernard, command):
		#update direction by forward
		#front
		if bernard.dir == 0:
			bernard.x += 1
			bernard.sx += 1
		#back
		if bernard.dir == 180 or bernard.dir == -180:
			bernard.x -= 1
			bernard.sx -= 1
		#right
		if bernard.dir == 90:
			bernard.y += 1
			bernard.sy += 1
		#left
		if bernard.dir == -90:
			bernard.y -= 1
			bernard.sy -= 1
		print("avance")
		print("pos x {} y {} spos {} {}".format(bernard.x, bernard.y, bernard.sx, bernard.sy))

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

from command import C, S, Command


# append l'action demandée dans une queue de Command
# repeat	: le nombre de fois ou elle sera executée
# element	: string ou array lié(e) à la commande
#	pour chaque element de l'array une commande sera executée avec buf = array[i]
#	ça permet d'envoyer une list de ressources à poser/prendre
def	compute_action(bernard, id, repeat = 1, element = None):
	bernard.needs[id].update(state = S.NEEDED)
	command = Command(id = id)
	command.repeat = repeat
	command.buf = element
	bernard.actions.append(command)

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

#WIP
def	goto_index(bernard, index):
	targetx, targety = getviewpos(index)
	x, y = bernard.x, bernard.y
	front, back, right, left = 0, 0, 0, 0
	#on stock les déplacements nécessaire pour rejoindre bêtement la pos target
	while x < targetx:
		x += 1
		front += 1
	while x > targetx:
		x -=1
		back += 1
	while y < targety:
		y += 1
		right += 1
	while y > targety:
		y -= 1
		left += 1
	#on push les mouvements nécessaire dans une queue à executer
	if front > 0:
		compute_action(bernard, C.AVANCE, front)
	if back > 0:
		compute_action(bernard, C.DROITE, 2)
		compute_action(bernard, C.AVANCE, back)
		compute_action(bernard, C.GAUCHE, 2)
	if right > 0:
		compute_action(bernard, C.DROITE, 1)
		compute_action(bernard, C.AVANCE, right)
		compute_action(bernard, C.GAUCHE, 1)
	if left > 0:
		compute_action(bernard, C.GAUCHE, 1)
		compute_action(bernard, C.AVANCE, left)
		compute_action(bernard, C.DROITE, 1)


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

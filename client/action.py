from command import C


def	getviewindex(x, y):
	index = 0
	middle = x * (x + 1)
	if y < 0:
		index = middle - y
	else:
		index = middle + y
	return index

def	outofview(x, y, lvl):
	range = lvl
	if x < 0 or x > range:
		return True
	range = abs(lvl - lvl - x)
	if y < -range or y > range:
		return True
	return False

class	Action:
	def	__init__(self):
		pass

	def	voir(bernard, command):
		#WIP
		if bernard.te == 0:
			bernard.te = bernard.ticks - bernard.t
		####
		bernard.view = []
		#on reset la position du joueur
		bernard.x, bernard.y = 0, 0
		for loot in command.response:
			bernard.view.append(loot)
		bernard.view_size = len(bernard.view)
		print("voir", bernard.view)

	def	inventaire(bernard, command):
		bernard.inventory = command.response

	def	prend(bernard, command):
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] = bernard.inventory[element] + 1
		viewcase = bernard.view[getviewindex(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] = viewcase[command.buf] - 1
		print("prend", command.buf)

	def	pose(bernard, command):
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] = bernard.inventory[element] - 1
		viewcase = bernard.view[getviewindex(bernard.x, bernard.y)]
		if command.buf in viewcase:
			viewcase[command.buf] = viewcase[command.buf] + 1
		print("pose", command.buf)

	def	droite(bernard, command):
		bernard.dir = bernard.dir + 90
		if bernard.dir == 270:
			bernard.dir = -90
		print("droite", bernard.dir)

	def	gauche(bernard, command):
		bernard.dir = bernard.dir - 90
		if bernard.dir == -270:
			bernard.dir = 90
		print("gauche", bernard.dir)

	def	avance(bernard, command):
		#front
		if bernard.dir == 0:
			bernard.x = bernard.x + 1
			bernard.sx = bernard.sx + 1
		#back
		if bernard.dir == 180 or bernard.dir == -180:
			bernard.x = bernard.x - 1
			bernard.sx = bernard.sx - 1
		#right
		if bernard.dir == 90:
			bernard.y = bernard.y + 1
			bernard.sy = bernard.sy + 1
		#left
		if bernard.dir == -90:
			bernard.y = bernard.y - 1
			bernard.sy = bernard.sy - 1
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

import math


def	view_check_neighbors(bernard, x, y, item):
	#si out of view on sort
	if outofview(bernard, x, y) == True:
		return None
	index = view_index(bernard.dir, x, y)
	#si deja visitée on sort
	if "visited" in bernard.view[index] and bernard.view[index]["visited"] == True:
		return None
	#has been visited
	bernard.view[index]["visited"] = True
	#si item sur la case et item > 0 on return l'index
	if item in bernard.view[index] and bernard.view[index][item] > 0:
		return index
	#recursion sur les 4 neighbors si result is not None: result is index
	result = view_check_neighbors(bernard, x + 1, y, item)
	if result is not None:
		return result
	result = view_check_neighbors(bernard, x - 1, y, item)
	if result is not None:
		return result
	result = view_check_neighbors(bernard, x, y + 1, item)
	if result is not None:
		return result
	return view_check_neighbors(bernard, x, y - 1, item)

#find l'item le plus proche du joueur
def	view_find(bernard, item):
	index = view_check_neighbors(bernard, bernard.x, bernard.y, item)
	#reset visited state
	for loot in bernard.view:
		if "visited" in loot:
			loot["visited"] = False
	return index


def	view_distance(dir, a ,b):
	ax, ay = view_pos(dir, a)
	bx, by = view_pos(dir, b)

	deltax = bx - ax
	deltax *= deltax

	deltay = by - ay
	deltay *= deltay

	return math.sqrt(deltax + deltay)

def	rotate_pos(dir, x, y):
	nx, ny = x, y

	if dir == 90:
		nx = y
		ny = -x
	elif dir == 180 or dir == -180:
		nx = -x
		ny = -y
	elif dir == -90:
		nx = -y
		ny = x
	return nx, ny

#return the index of bernard in view with x, y
def	view_index(dir, x, y):
	x, y = rotate_pos(dir, x, y)
	index = 0
	middle = y * (y + 1)
	index = middle + x
	return index

#return the pos of bernard with view index
def	view_pos(dir, index):
	x, y = 0, 0
	#if index == 0, pox == 0, 0
	if index <= 0:
		return x, y
	start, end = 0, 0
	for i in range(1, 8):
		lvl = i + 1
		middle = lvl * i
		start = middle - lvl + 1
		end = middle + lvl - 1
		if index > start and index < end:
			break
	y = lvl - 1
	x = -y
	for i in range(start, end):
		if index == view_index(0, x, y):
			break
		x += 1
	x, y = rotate_pos(dir, x, y)
	return x, y

#return true if x, y are out of bernard.view
def	outofview(bernard, x, y):
	index = view_index(bernard.dir, x, y)
	if index < 0 or index > bernard.view_size - 1:
		return True
	return False

import math


def	view_check_neighbors(view, lvl, x, y, item):
	#si out of view on sort
	if outofview(x, y, lvl) == True:
		return None
	index = view_index(x, y)
	#si deja visitée on sort
	if "visited" in view[index] and view[index]["visited"] == True:
		return None
	#has been visited
	view[index]["visited"] = True
	#si item sur la case et item > 0 on return l'index
	if item in view[index] and view[index][item] > 0:
		return index
	#recursion sur les 4 neighbors si result is not None: result is index
	result = view_check_neighbors(view, lvl, x + 1, y, item)
	if result is not None:
		return result
	result = view_check_neighbors(view, lvl, x - 1, y, item)
	if result is not None:
		return result
	result = view_check_neighbors(view, lvl, x, y + 1, item)
	if result is not None:
		return result
	result = view_check_neighbors(view, lvl, x, y - 1, item)
	if result is not None:
		return result
	return None

#find l'item le plus proche du joueur
def	view_find(bernard, view, item):
	index = view_check_neighbors(view, bernard.lvl, bernard.x, bernard.y, item)
	#reset visited state
	for loot in view:
		if "visited" in loot:
			loot["visited"] = False
	return index


def	view_distance(a ,b):
	ax, ay = view_pos(a)
	bx, by = view_pos(b)

	deltax = bx - ax
	deltax *= deltax

	deltay = by - ay
	deltay *= deltay

	return math.sqrt(deltax + deltay)

#return the index of bernard in view with x, y
def	view_index(x, y):
	index = 0
	middle = y * (y + 1)
	index = middle + x
	return index

#return the pos of bernard in view with index
def	view_pos(index):
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
	y = lvl - 1
	x = -y
	for i in range(start, end):
		if index == view_index(x, y):
			break
		x += 1
	return x, y

#return true if x, y are out of bernard.view
def	outofview(x, y, lvl):
	offset = lvl
	if y < 0 or y > offset:
		return True
	offset = abs(y)
	if x < -offset or x > offset:
		return True
	return False

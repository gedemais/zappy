import math


def	view_check_neighbors(bernard, x, y, item):
	#si out of view on sort
	if outofview(bernard, x, y) == True:
		return None
	index = view_index(x, y)
	#si deja visitée on sort
	if ("visited" in bernard.view[index] and bernard.view[index]["visited"] == True)\
			or ("player" in bernard.view[index] and bernard.view[index]["player"] > 1):
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

#WIP
def	view_distance(a ,b):
	ax, ay = view_pos(a)
	bx, by = view_pos(b)

	if ax > bx:
		tmp = bx
		bx = ax
		ax = tmp
	if ay > by:
		tmp = by
		by = ay
		ay = tmp

	deltax = bx - ax
	deltax *= deltax

	deltay = by - ay
	deltay *= deltay

	return math.sqrt(deltax + deltay)

#return the index of bernard in view with x, y
def	view_index(x, y):
	index = 0
	if x == 0 and y == 0:
		return index
	y = abs(y)
	middle = y * (y + 1)
	index = middle + x
	return index

#return the pos of bernard with view index
def	view_pos(index):
	x, y = 0, 0
	#if index == 0, pox == 0, 0
	if index <= 0:
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
	x = -lvl
	for i in range(start + 1, end):
		x += 1
		if index == view_index(x, y):
			break
	return x, y

#return true if x, y are out of bernard.view
def	outofview(bernard, x, y):
	index = view_index(x, y)

	return index < 0 or index > bernard.view_size - 1

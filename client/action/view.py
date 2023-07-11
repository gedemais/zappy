def	view_check_neighbors(bernard, x, y, item):
	#si out of view on sort
	if outofview(x, y, bernard.lvl) == True:
		return None
	index = view_index(x, y)
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
	result = view_check_neighbors(bernard, x, y - 1, item)
	if result is not None:
		return result
	return None

def	view_find(bernard, item):
	index = view_check_neighbors(bernard, bernard.x, bernard.y, item)
	#reset visited state
	for loot in bernard.view:
		if "visited" in loot:
			loot["visited"] = False
	return index

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

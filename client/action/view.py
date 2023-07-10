#return the index of bernard in view with x, y
def	getviewindex(x, y):
	index = 0
	middle = y * (y + 1)
	index = middle + x
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
	y = lvl - 1
	x = -y
	for i in range(start, end):
		if index == getviewindex(x, y):
			break
		x += 1
	return x, y

#return true if x, y are out of bernard.view
def	outofview(x, y, lvl):
	offset = lvl
	if y < 0 or y > offset:
		return True
	offset = abs(lvl - lvl - x)
	if x < -offset or x > offset:
		return True
	return False

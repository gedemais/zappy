from action.callback import compute_action
from action.view import view_pos
from utils.command import C


#WIP
#on stock les déplacements nécessaire pour rejoindre bêtement la pos target
def	dummy_path(x, y, tx, ty):
	front, back, right, left = 0, 0, 0, 0

	while y < ty:
		y += 1
		front += 1
	while y > ty:
		y -= 1
		back += 1
	while x < tx:
		x += 1
		right += 1
	while x > tx:
		x -=1
		left += 1
	return front, back, right, left

#WIP
def	set_direction(bernard, tx, ty):
	if (ty > 0 and tx > 0) or (ty > 0 and tx < 0):
		if bernard.dir == 90:
			compute_action(bernard, C.GAUCHE, 1)
		if bernard.dir == -90:
			compute_action(bernard, C.DROITE, 1)
		if bernard.dir == 180 or bernard.dir == -180:
			compute_action(bernard, C.DROITE, 2)
	if (ty < 0 and tx > 0) or (ty < 0 and tx < 0):
		if bernard.dir == 90:
			compute_action(bernard, C.DROITE, 1)
		if bernard.dir == -90:
			compute_action(bernard, C.GAUCHE, 1)
		if bernard.dir == 0:
			compute_action(bernard, C.DROITE, 2)

#WIP
def	compute_path(bernard, front, back, right, left):
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

#WIP prendre une list de target en parametre et renvoyer le le chemin qui les relit le plus cours
def	goto_pos(bernard, targetx, targety):
	#si bernard est déjà à la pos indiquée
	if bernard.x == targetx and bernard.y == targety:
		return
	#WIP
	front, back, right, left = dummy_path(bernard.x, bernard.y, targetx, targety)
	#il faut verifier la direction du joueur et la set a front
	set_direction(bernard, targetx, targety)
	#on push les mouvements nécessaire dans une queue à executer
	compute_path(bernard, front, back, right, left)

def	goto_index(bernard, index):
	targetx, targety = view_pos(index)
	goto_pos(bernard, targetx, targety)

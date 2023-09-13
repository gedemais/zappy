import heapq

from action.utils import compute_action, case_value
from action.view import view_pos, view_index
from action.incant import incant_total
from utils.command import C


#on stock les déplacements nécessaire pour rejoindre bêtement la pos target
def		dummy_path(x, y, tx, ty):
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

def		compute_path(bernard, front, back, right, left):
	#on push les mouvements nécessaire dans une queue à executer
	if front > 0:
		compute_action(bernard, C.AVANCE, front)
		bernard.y += front
	if back > 0:
		compute_action(bernard, C.DROITE, 2)
		compute_action(bernard, C.AVANCE, back)
		compute_action(bernard, C.GAUCHE, 2)
		bernard.y -= back
	if right > 0:
		compute_action(bernard, C.DROITE, 1)
		compute_action(bernard, C.AVANCE, right)
		compute_action(bernard, C.GAUCHE, 1)
		bernard.x += right
	if left > 0:
		compute_action(bernard, C.GAUCHE, 1)
		compute_action(bernard, C.AVANCE, left)
		compute_action(bernard, C.DROITE, 1)
		bernard.x -= left

#WIP prendre une list de target en parametre et renvoyer le le chemin qui les relit le plus cours
def		goto_pos(bernard, targetx, targety):
	#si bernard est déjà à la pos indiquée
	if bernard.x == targetx and bernard.y == targety:
		return
	front, back, right, left = dummy_path(bernard.x, bernard.y, targetx, targety)
	#on push les mouvements nécessaire dans une queue à executer
	compute_path(bernard, front, back, right, left)

def		goto_index(bernard, index):
	targetx, targety = view_pos(index)
	goto_pos(bernard, targetx, targety)
	print("going to {}".format(index))

#WIP algo de meilleur chemin pour la cible
#prendre la premiere case issue de lalgo et envoyer le bot dessus
#refaire jusqua avoir ateint la case finale
# class	Node:
# 	def __init__(self, x, y, parent = None):
# 		self.x = x
# 		self.y = y
# 		self.parent = parent
# 		self.g = 0		# coût depuis le point de départ
# 		self.h = 0		# estimation du coût jusqu'à la destination
# 		self.f = 0		# coût total (f = g + h)
# 		self.value = 0	# valeur de la case

# 	def __lt__(self, other):
# 		return self.f < other.f

# def		heuristic(node, goal):
# 	return abs(node.x - goal.x) + abs(node.y - goal.y)

# def		find_best_path(start, goal, values):
# 	open_set = []
# 	closed_set = set()

# 	start_node = Node(start[0], start[1])
# 	goal_node = Node(goal[0], goal[1])
# 	heapq.heappush(open_set, start_node)
# 	while open_set:
# 		current_node = heapq.heappop(open_set)
# 		if current_node.x == goal_node.x and current_node.y == goal_node.y:
# 			# Nous avons atteint la destination
# 			path = []
# 			while current_node:
# 				path.insert(0, (current_node.x, current_node.y))
# 				current_node = current_node.parent
# 			return path
# 		closed_set.add((current_node.x, current_node.y))
# 		for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
# 			new_x, new_y = current_node.x + dx, current_node.y + dy
# 			if (\
# 				0 <= new_x < len(values)\
# 			and 0 <= new_y < len(values[0])\
# 			and (new_x, new_y) not in closed_set\
# 			):
# 				new_node = Node(new_x, new_y, current_node)
# 				new_node.g = current_node.g + 1
# 				new_node.h = heuristic(new_node, goal_node)
# 				new_node.value = values[new_x][new_y]
# 				new_node.f = new_node.g + new_node.h + new_node.value
# 				for node in open_set:
# 					if node.x == new_node.x and node.y == new_node.y and node.f < new_node.f:
# 						break
# 					else:
# 						heapq.heappush(open_set, new_node)
# 	# Aucun chemin trouvé
# 	return None

def		goto_path(bernard, index):
	print("going to {}".format(index))
	goto_index(bernard, index)
	# mx = bernard.lvl * 2
	# my = bernard.lvl
	# viewmap = [[-1 for _ in range(mx + 1)] for _ in range(my + 1)]
	# values = incant_total(bernard, bernard.rushlvl)
	# len = bernard.view_size - 1

	# fi = 0
	# while (my >= 0):
	# 	x = mx
	# 	while (x >= 0):
	# 		v = -1
	# 		if x >= fi and x < (mx + 1) - fi:
	# 			v = case_value(bernard.view[len], values)
	# 			len = len - 1
	# 			if v == 0:
	# 				v = 1
	# 		viewmap[my][x] = v
	# 		x = x - 1
	# 	fi = fi + 1
	# 	my = my - 1

	# print(viewmap)
	# x, y = view_pos(index)
	# path = find_best_path((bernard.x, bernard.y), (x, y), viewmap)
	# print("====================================== PATH")
	# print(path)
	# print("======================================")

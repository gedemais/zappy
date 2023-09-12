import os, time

from utils.command import C
from action.utils import compute_action, send_broadcast


class	Callback:
	def	__init__(self):
		pass

	def	voir(bernard, command):
		bernard.view = []
		#on reset la position du joueur
		bernard.x, bernard.y = 0, 0
		for loot in command.response:
			bernard.view.append(loot)
		bernard.view_size = len(bernard.view)
		# print("[ voir ]", bernard.view)

	def	inventaire(bernard, command):
		#last inventory update
		bernard.last_inventory = bernard.t
		bernard.inventory = command.response
		# print("[ inventaire ]", bernard.inventory)

	def	prend(bernard, command):
		#update inventory
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] += 1
		# 		index = element
		# print("[ prend ] - {}".format(command.buf))
		# print("inventaire: {} -> {}".format(bernard.inventory[index] - 1, bernard.inventory[index]))

	def	pose(bernard, command):
		#update inventory
		for element in bernard.inventory:
			if element == command.buf:
				bernard.inventory[element] -= 1
		# 		index = element
		# print("[ pose ] - {}".format(command.buf))
		# print("inventaire: {} -> {}".format(bernard.inventory[index] + 1, bernard.inventory[index]))

	def	droite(bernard, command):
		pass

	def	gauche(bernard, command):
		pass

	def	avance(bernard, command):
		pass

	def	connect_nbr(bernard, command):
		if len(command.response) > 5:
			return
		split = command.response.split(", ")
		one = split[0].strip()
		two = split[1].strip()
		bernard.team_slot = int(one)
		bernard.team_total = int(two)
		if bernard.id is None:
			bernard.id = bernard.team_total

	def incantation(bernard, command):
		bernard.lvl += 1
		print("---------------------------------- LVL [ {} ]".format(bernard.lvl))

	def	fork(bernard, command):
		time.sleep(3.33)
		os.system("python3 main.py --host {} --port {} --team_name {} &".format(bernard.host, bernard.port, bernard.team_name))
		bernard.hatched = True
		bernard.last_hatch = bernard.t

	def	expulse():
		pass

	def	broadcast(bernard, command):
		#last broadcast sent
		bernard.last_broadcast = bernard.t

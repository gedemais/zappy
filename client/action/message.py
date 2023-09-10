from action.utils import compute_action
from utils.command import C


class	Message:
	def	__init__(self):
		pass

	#format: message <K>,<texte>
	def message(bernard, message):
		split = message.split(',')
		message = split[1].strip()
		split = split[0].split(' ')
		dir = int(split[1].strip())
		print("from: {} - message: {}".format(dir, message))
		if "Come to your leader !" in message:
			if bernard.leader == -1:
				bernard.leader = None
				return
			bernard.leader = dir
			print("joining leader at {}".format(dir))
		elif "I just hatched an egg !" in message:
			bernard.team_total += 1
		elif "The king is dead, long live the king !" in message:
			bernard.leader = None

	#format: deplacement <K>
	# 0 : front, 1 : right, 2 : back, 3 : left
	#the kick message reset bernard's brain
	def	kick(bernard, message):
		print("bot has been kick")
		split = message.split(' ')
		kick_origin = int(split[1].strip())
		x, y = bernard.x, bernard.y

		#si 0 faut allez derriere
		if kick_origin == 0:
			originx, originy = bernard.x, bernard.y + 1
			bernard.y -= 1
			bernard.sy -= 1
		#si 1 faut allez a droite
		elif kick_origin == 1:
			originx, originy = bernard.x - 1, bernard.y
			bernard.x += 1
			bernard.sx += 1
		#si 2 faut allez devant
		elif kick_origin == 2:
			originx, originy = bernard.x, bernard.y - 1
			bernard.y += 1
			bernard.sy += 1
		#si 3 faut allez a gauche
		elif kick_origin == 3:
			originx, originy = bernard.x + 1, bernard.y
			bernard.x -= 1
			bernard.sx -= 1
		print("kick origin: {}, {}".format(originx, originy))
		print("pos: {}, {} -> {}, {}".format(x, y, bernard.x, bernard.y))
		#comme on a été poussé alors il faut revoir les commandes en cours
		bernard.brain.reset()
		bernard.view = []

	def	start(bernard, message):
		print(message)

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

	#format: deplacement <K>
	# 1 is front
	# 2 1 8
	# 3 X 7
	# 4 5 6
	#the kick message reset bernard's breain
	def	kick(bernard, message):
		print("bot has been kick")
		split = message.split(' ')
		kick_origin = int(split[1].strip())
		x, y = bernard.x, bernard.y

		#si 1 faut allez derriere
		if kick_origin == 1:
			originx, originy = bernard.x, bernard.y + 1
			bernard.y -= 1
			bernard.sy -= 1
		#si 3 faut allez a droite
		elif kick_origin == 3:
			originx, originy = bernard.x - 1, bernard.y
			bernard.x += 1
			bernard.sx += 1
		#si 5 faut allez devant
		elif kick_origin == 5:
			originx, originy = bernard.x, bernard.y - 1
			bernard.y += 1
			bernard.sy += 1
		#si 7 faut allez a gauche
		elif kick_origin == 7:
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

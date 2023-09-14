class	Message:
	def	__init__(self):
		pass

	#format: message <K>,<texte>
	def message(bernard, message):
		split = message.split(',')
		message = split[1].strip()
		split = split[0].split(' ')
		dir = int(split[1].strip())
		if "Come to your leader !" in message:
			bernard.leader = dir
			bernard.leader_contact = bernard.t
			bernard.leader_meet = True
			if bernard.leader == -1:
				bernard.doppleganger = True
		elif "I'm your leader !" in message\
				or "I'm the true leader !" in message:
			bernard.leader = dir
			bernard.leader_contact = bernard.t
		elif "I just hatched an egg !" in message:
			pass
		elif "My level is : " in message:
			split = message.split(':')
			lvl = int(split[1].strip())
			if bernard.lvl > lvl + 1:
				bernard.suicide = True
		if dir > 0:
			bernard.bdir.append(dir)

	#format: deplacement <K>
	# 0 : front, 1 : right, 2 : back, 3 : left
	#the kick message reset bernard's brain
	def	kick(bernard, message):
		#comme on a été poussé alors il faut revoir les commandes en cours
		bernard.brain.reset()
		bernard.view = []

class	Message:
	def	__init__(self):
		pass

	#format: message <K>,<texte>
	def message(bernard, message):
		split = message.split(',')
		message = split[1].strip()
		split = split[0].split(' ')
		dir = split[1].strip()
		print("from: {} - message: {}".format(dir, message))

	#format: deplacement <K>\n
	def	kick(bernard, message):
		print("bot has been kick")

	#format: niveau actuel : K
	def	level(bernard, message):
		split = message.split(':')
		bernard.lvl = split[1].strip()
		print("---------------------------------- LVL [ {} ]".format(bernard.lvl))

	def	start(bernard, message):
		print(message)

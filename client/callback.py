from command import C


class	Callback:
	def	connect_nbr(cmd):
		cmd.id = C.INVENTAIRE

	def	inventaire(cmd):
		cmd.id = C.VOIR

	def	voir(cmd):
		cmd.id = C.AVANCE

	def	avance(cmd):
		print(cmd.response)

	def	droite(cmd):
		print(cmd.response)

	def	gauche(cmd):
		print(cmd.response)

	def	prend(cmd):
		print(cmd.response)

	def	pose(cmd):
		print(cmd.response)

	def	incantation(cmd):
		print(cmd.response)

	def	fork(cmd):
		print(cmd.response)

	def	expulse(cmd):
		print(cmd.response)

	def	broadcast(cmd):
		print(cmd.response)

	def	run(cmd):
		cmd.callback(cmd)

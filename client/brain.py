from command import C, Command


class	Brain:
	def	__init__(self):
		self.name = "brain"

	def	simpe():
		return ([
			Command(id = C.AVANCE),
			Command(id = C.AVANCE),
			Command(id = C.AVANCE),
			Command(id = C.DROITE),
			Command(id = C.PREND, buf = "nourriture")
		])

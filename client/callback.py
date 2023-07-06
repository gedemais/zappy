from task import T


class	Callback:
	def	connect_nbr(task):
		print(task["response"])

	def	inventaire(task):
		print(task["response"])

	def	voir(task):
		print(task["response"])

	def	avance(task):
		print(task["response"])

	def	droite(task):
		print(task["response"])

	def	gauche(task):
		print(task["response"])

	def	prend(task):
		print(task["response"])

	def	pose(task):
		print(task["response"])

	def	incantation(task):
		print(task["response"])

	def	fork(task):
		print(task["response"])

	def	expulse(task):
		print(task["response"])

	def	broadcast(task):
		print(task["response"])

	def	run(task):
		if  task["response"] != None and task["callback"] != None:
			print("callback", task["command"].strip())
			task["callback"](task)

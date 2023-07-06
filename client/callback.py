class	Callback:
	def	connect_nbr(response):
		print(response)

	def	inventaire(response):
		print(response)

	def	voir(response):
		print(response)

	def	run(task):
		if  task["response"] != None and task["callback"] != None:
			print("callback", task["command"].strip())
			task["callback"](task["response"])

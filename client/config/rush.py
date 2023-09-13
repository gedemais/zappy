from action.utils import is_blind
from action.incant import incant_possible, incant_total
from config.incantation import Incantation
from config.collect import Collect


class	Rush:
	def	__init__(self):
		pass

	def	run(bernard, lvl):
		if is_blind(bernard) == True or bernard.lvl > lvl - 1:
			return
		if incant_possible(bernard, False) == False:
			Collect.run(bernard, incant_total(bernard, lvl))
		else:
			Incantation.run(bernard)

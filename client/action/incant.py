from action.view import view_index


#tableau de prerequis pour lvl up
require = [
	#1 - 2
	{ "linemate" : 1, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 1 },
	#2 - 3
	{ "linemate" : 1, "deraumere" : 1, "sibur" : 1, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 2 },
	#3 - 4
	{ "linemate" : 2, "deraumere" : 0, "sibur" : 1, "mendiane" : 0, "phiras" : 2, "thystame" : 0, "player" : 2 },
	#4 - 5
	{ "linemate" : 1, "deraumere" : 1, "sibur" : 2, "mendiane" : 0, "phiras" : 1, "thystame" : 0, "player" : 4 },
	#5 - 6
	{ "linemate" : 1, "deraumere" : 2, "sibur" : 1, "mendiane" : 3, "phiras" : 0, "thystame" : 0, "player" : 4 },
	#6 - 7
	{ "linemate" : 1, "deraumere" : 2, "sibur" : 3, "mendiane" : 0, "phiras" : 1, "thystame" : 0, "player" : 6 },
	#7- 8
	{ "linemate" : 2, "deraumere" : 2, "sibur" : 2, "mendiane" : 2, "phiras" : 2, "thystame" : 1, "player" : 6 },
]

#WIP
#return le nb de loot total a avoir pour passer du lvlmin au lvlmax
def		incant_total(bernard, lvlmax):
	needs = { "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0 }

	#on additionne tout les loots entre le lvlmin et le lvlmax
	for i in range(bernard.lvl - 1, lvlmax - 1):
		loots = require[i]
		for item in loots:
			if "player" not in item and loots[item] > 0:
				needs[item] += loots[item]
	#il faut soustraire l'inventaire
	for element in bernard.inventory:
		if "nourriture" in element or "ttl" in element:
			continue
		if element in needs and bernard.inventory[element] > 0:
			needs[element] -= bernard.inventory[element]
	return needs

#return True if can incant else False
#if case is True incant possible count items on the ground as well
def		incant_possible(bernard, case = False):
	loots_to_incant = require[bernard.lvl - 1]
	bernardindex = view_index(bernard.x, bernard.y)
	viewcase = bernard.view[bernardindex]

	for element in bernard.inventory:
		if "nourriture" in element or "ttl" in element:
			continue
		offset = 0
		if case == True and element in viewcase and viewcase[element] > 0:
			offset = viewcase[element]
		if bernard.inventory[element] + offset < loots_to_incant[element]:
			return False
		if "player" in viewcase and viewcase["player"] < require[bernard.lvl - 1]["player"]:
			return False
	return True

#return un array qui contient le nb de loot à poser à terre pour lancer le lvl up
def		incant_put(bernard):
	needs = { "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 0 }
	loots_to_incant = require[bernard.lvl - 1]
	index = view_index(bernard.x, bernard.y)

	for item in loots_to_incant:
		if "player" not in item and loots_to_incant[item] > 0:
			#si les loot au sol sont < au prérequis
			if item in bernard.view[index] and bernard.view[index][item] > 0:
				if bernard.view[index][item] < loots_to_incant[item]:
					#on fait la diff entre les ressources au sol et celles présente dans l'inventaire
					needs[item] = loots_to_incant[item] - bernard.view[index][item]
			else:
				needs[item] = loots_to_incant[item]
	return needs

#return un array contenant tout les loots nécessaire au lvl up (inventaire soustrait)
def		incant_need(bernard):
	needs = { "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 0 }
	loots_to_incant = require[bernard.lvl - 1]

	if incant_possible(bernard) == True:
		return needs
	else:
		for element in bernard.inventory:
			if "nourriture" in element or "ttl" in element:
				continue
			#si on a pas assez de ressources dans l'inventaire
			if bernard.inventory[element] < loots_to_incant[element]:
				needs[element] = loots_to_incant[element] - bernard.inventory[element]
	return needs

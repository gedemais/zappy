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

#return True if can incant else False
def		incant_possible(lvl, inventory):
	loots_to_incant = require[lvl - 1]

	for loot in inventory:
		if "nourriture" in loot or "ttl" in loot:
			continue
		if inventory[loot] < loots_to_incant[loot]:
			return False
	return True

#return un array qui contient le nb de loot à poser à terre pour lancer le lvl up
def		incant_put(bernard):
	needs = { "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 0 }
	loots_to_incant = require[bernard.lvl - 1]

	for loot in bernard.inventory:
		if "nourriture" in loot or "ttl" in loot:
			continue
		#si les loot au sol sont < au prérequis
		if loot in bernard.view[0] and bernard.view[0][loot] < loots_to_incant[loot]:
			#on deande la diff entre les ressources au sol et celles présente dans l'inventaire
			needs[loot] = bernard.inventory[loot] - bernard.view[0][loot]
	return needs

#return un array contenant tout les loots nécessaire au lvl up
def		incant_need(lvl, inventory):
	needs = { "linemate" : 0, "deraumere" : 0, "sibur" : 0, "mendiane" : 0, "phiras" : 0, "thystame" : 0, "player" : 0 }
	loots_to_incant = require[lvl - 1]

	if incant_possible(lvl, inventory) == True:
		return needs
	else:
		for loot in inventory:
			if "nourriture" in loot or "ttl" in loot:
				continue
			#si on a pas assez de ressources dans l'inventaire
			if inventory[loot] < loots_to_incant[loot]:
				needs[loot] = loots_to_incant[loot] - inventory[loot]
	return needs

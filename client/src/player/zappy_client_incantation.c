#include "zappy_client.h"


static void	add_inventaires(t_inventaire *result, t_inventaire first, t_inventaire second)
{
	int	*ptr_first = (int *)&first;
	int	*ptr_second = (int *)&second;
	int	*ptr_result = (int *)result;

	memset(result, 0, sizeof(t_inventaire));
	for (int i = 0; i < R_MAX - 1; i++) {
		ptr_result[i] = ptr_first[i] + ptr_second[i];
	}
} 

// return 1 if requirements are OK else 0
static int	ressources_requirement(t_inventaire inventaire, uint8_t lvl)
{
	int	r = 1;
	int	*ptr_inventaire = (int *)&inventaire;
	int	*ptr_requirement = (int *)&requirements[lvl];

	// i = 1 car on skip la ressource nourriture et
	// R_MAX - 1 pour ne pas prendre en compte le ttl
	for (int i = 1; i < R_MAX - 1; i++) {
		if (ptr_inventaire[i] < ptr_requirement[i]) {
			r = 0;
			break ;
		}
	}
	return (r);
}

// TODO : il faut poser les ressources à terre
static int	ressources_poser_requirement()
{
	int	r = 0;

	return (r);
}

// TODO
// return 1 if requirements are OK else 0
static int	players_requirement()
{
	int	r = 0;

	return (r);
}

// TODO
static int	zappy_incantation()
{
	int	r = 0;

	return (r);
}

int			zappy_client_incantation(zappy_client_t *client)
{
	int				r = 0;
	t_inventaire	inventaire_player, inventaire_case, tmp;

	inventaire_player = client->player.inventaire[client->player.id];
	// step 1 : verifier si le joueur a assez de nourriture pour survivre a 1,5 incantations
	// incantation 300t, 1 food = 126t il faut minimum 3 foods
	if (inventaire_player.nourriture < 4) {
		client->task = PLAYER_TASK_LOOT;
		return (r);
	}
	// TODO : get inventaire_case
	inventaire_case = inventaire_player;
	// step 2 : on additionne les ressources sur la case et  celle du joueur
	add_inventaires(&tmp, inventaire_player, inventaire_case);
	// on check si l'inventaire possede les ressources necessaire
	if (ressources_requirement(tmp, client->player.lvl) == true) {
		// step 3 : on verifie si les ressources sont déjà sur la case
		if (ressources_requirement(inventaire_case, client->player.lvl) == false) {
			// TODO
			r = ressources_poser_requirement();
			return (r);
		}
		// TODO
		// step 4 : verifier si le nb de joueurs requis sont présent sur la case
		if (players_requirement() == true) {
			// TODO
			// step 5 : lancer l'incantation de lvl up
			r = zappy_incantation();
		} else {
			// il faut appeler des joueurs
			client->player.broadcast = BROADCAST_PLAYER;
			client->task = PLAYER_TASK_BROADCAST;
		}
	} else {
		client->task = PLAYER_TASK_LOOT;
	}
	return (r);
}

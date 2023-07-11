#include "main.h"

static bool	check_resources(t_env *env, t_player *p)
{
	t_tile	*tile;

	tile = &env->world.map[p->tile_y][p->tile_x];
	for (unsigned int i = 0; i < LOOT_MAX; i++)
	{
		if (tile->content[i] < lvl_up_requirements[p->level - 1][i])
			return (false);
	}
	return (true);
}

static bool	check_players(t_env *env, t_player *p, t_dynarray players)
{
	t_team			*t;
	t_player		*pl;
	unsigned int	nb_players = 0;
	bool			coords;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		for (int j = 0; j < env->world.teams.nb_cells; j++)
		{
			pl = dyacc(&t->players, j);
			if (dynarray_push(players, pl))
				return (ERR_MALLOC_FAILED);
			coords = (bool)(pl->tile_x == p->tile_x && pl->tile_y == p->tile_y);
			if (coords && pl->level == p->level)
				nb_players++;
		}
	}
	return (nb_players == lvl_up_requirements[p->level - 1][LOOT_MAX]);
}

static void	consume_resources(t_env *env, t_player *p)
{
	t_tile	*tile;

	tile = &env->world.map[p->tile_y][p->tile_x];
	for (unsigned int i = 0; i < LOOT_MAX; i++)
	{
		for (unsigned int n = 0; n < lvl_up_requirements[p->level - 1][i]; n++)
			tile->content[i]--;
	}
}

uint8_t	cmd_incantation(t_env *env, t_player *p, bool send_response)
{
	// L'ensemble de cette fonction doit etre execute a la reception de la commande.
	// La veritable resolution de la commande incantation reside en l'application
	// des differentes montees de niveau des joueurs concernes par cette derniere.
	// Une liste des joueurs censes level up doit etre creee par cette fonction,
	// en l'etat, puis resolue par sa version finale.
	t_dynarray	players;

	if (!check_resources(env, p)
		|| !check_players(env, p, &players))
		return (send_response ? send_ko(env, p) : ERR_NONE);

	consume_resources(env, p);

	// Plan level up for each concerned player
	plan_level_up(env, players);
	return (send_response ? send_ok(env, p) : ERR_NONE);
}


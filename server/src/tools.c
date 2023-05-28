#include "main.h"

t_player	*get_team_client(t_env *env, int client_fd)
{
	t_player	*p;
	t_team		*team;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < team->players.nb_cells; j++)
		{
			p = dyacc(&team->players, j);
			if (*p->connection == client_fd)
				return (p);
		}
	}

	return (NULL);
}

t_player	*get_pending_client(t_env *env, int client_fd)
{
	t_player	*p;
	t_team		*team;

	team = &env->world.pending;

	for (int i = 0; i < team->players.nb_cells; i++)
	{
		p = dyacc(&team->players, i);
		if (*p->connection == client_fd)
			return (p);
	}

	return (NULL);
}

t_team	*get_client_team(t_env *env, int client_fd)
{
	t_team		*team;
	t_player	*p;

	for (int t = 0; t < env->world.teams.nb_cells; t++)
	{
		team = dyacc(&env->world.teams, t);
		for (int i = 0; i < team->players.nb_cells; i++)
		{
			p = dyacc(&team->players, i);
			if (*p->connection == client_fd)
				return (team);
		}
	}
	return (NULL);
}


uint8_t	remove_player_from_tile(t_env *env, int x, int y)
{
	uint8_t		*loot;

	for (int i = 0; i < env->world.map[y][x].content.nb_cells; i++)
	{
		loot = dyacc(&env->world.map[y][x].content, i);
		if (*loot == 255)
		{
			if (extract_dynarray(&env->world.map[y][x].content, i))
				return (ERR_MALLOC_FAILED);
			break;
		}
	}
	return (ERR_NONE);
}



#include "main.h"

t_player	*get_player_from_tile(t_env *env, int x, int y)
{
	t_player	*p;
	t_team		*team;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < team->players.nb_cells; j++)
		{
			p = dyacc(&team->players, j);
			if (p->tile_x == x && p->tile_y == y)
				return (p);
		}
	}

	return (NULL);
}
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

uint8_t	remove_player_from_tile(t_env *env, int x, int y)
{
	if (env->world.map[y][x].content[LOOT_PLAYER] > 0)
		env->world.map[y][x].content[LOOT_PLAYER]--;
	return (ERR_NONE);
}

uint8_t	send_ko(t_env *env, t_player *p)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, "ko\n");
	response(env, p);
	return (response(env, p));
}

uint8_t	send_ok(t_env *env, t_player *p)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, "ok\n");
	return (response(env, p));
}

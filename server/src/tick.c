#include "main.h"

static void	check_game_start(t_env *env)
{
	t_team	*team;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		if (team->max_client - team->connected > 0)
			return ;
	}
	env->start = true;
	PUTTIME()
	fprintf(stderr, "[GAME_STARTED] Game started with %d teams of %d players !\n", env->world.teams.nb_cells, env->settings.max_connections);
}

static void	check_game_end(t_env *env)
{
	t_team		*team;
	t_player	*player;
	bool		won;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		won = (bool)team->players.nb_cells > 0;
		for (int p = 0; p < team->players.nb_cells; i++)
		{
			player = dyacc(&team->players, i);
			if (player->level < 8)
			{
				won = false;
				break ;
			}
		}

		if (won == true)
		{
			env->gindex = i;
			gevent_game_ended(env);
			printf("GAME WIN\n");
			exit(0);
		}
	}
}



uint8_t	tick(t_env *env)
{
	static uint64_t	n = 0;
	uint8_t			code;
	struct timeval	tick_start, tick_end;
	useconds_t		elapsed;

	gettimeofday(&tick_start, NULL);

	// LOGGING
	n++;

	if (n % env->settings.t == 0 && (code = update_inventories(env)))
		return (code);

#ifdef MACOS
	fprintf(stderr, "============= TICK %llu =============\n", n);
#endif

#ifdef LINUX
	fprintf(stderr, "============= TICK %ld =============\n", n);
#endif

	teams_log(env, true);
	teams_log(env, false);
	env->start ? check_game_end(env) : check_game_start(env);

	if ((code = receipt(env)) != ERR_NONE
		|| (env->start && (code = update_players(env))) != ERR_NONE
		|| (code = update_eggs(env)) != ERR_NONE
		|| (code = update_commands(env)) != ERR_NONE)
		return (code);

	gettimeofday(&tick_end, NULL);
	elapsed = tick_end.tv_usec - tick_start.tv_usec;
	usleep(env->settings.tick_length - elapsed);
	fprintf(stderr, "Waiting %d microseconds before the start of the next cycle...\n", env->settings.tick_length - elapsed);
	return (ERR_NONE); //Leaks test
}


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
		won = team->players.nb_cells == 6;
		if (won)
			for (int p = 0; p < team->players.nb_cells; p++)
			{
				player = dyacc(&team->players, p);
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

static uint8_t	regenerate_food(t_env *env)
{
	int	x, y;

	FLUSH_GRESPONSE
	printf("SALOPE1\n");
	fflush(stdout);
	if (env->consumed_food < 0)
	{
		printf("SALOPE2\n");
		fflush(stdout);
		for (int32_t i = 0; i < env->consumed_food; i++)
		{
			printf("SALOPE3\n");
			fflush(stdout);
			x = rand() % env->settings.map_width;
			y = rand() % env->settings.map_height;
			env->world.map[y][x].content[LOOT_FOOD]++;
			if (env->graphical.team != 0)
				gcmd_block_content(env);
		}
	}

	printf("SALOPE4\n");
	fflush(stdout);
	env->consumed_food = 0;
	return (gresponse(env));
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

	if ((code = regenerate_food(env)))
		return (code);

	//if (n % env->settings.t == 0 && env->start
	//	&& (code = update_inventories(env)))
	//	return (code);

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

	if (elapsed < env->settings.tick_length)
		usleep(env->settings.tick_length - elapsed);
	fprintf(stderr, "Waiting %d microseconds before the start of the next cycle...\n", env->settings.tick_length - elapsed);
	return (ERR_NONE); //Leaks test
}


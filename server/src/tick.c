#include "main.h"

static void	check_game_start(t_env *env)
{
	t_team	*team;

	if (env->start)
		return;

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

uint8_t	tick(t_env *env)
{
	static uint64_t	n = 0;
	uint8_t			code;
	struct timeval	tick_start, tick_end;
	useconds_t		elapsed;

	gettimeofday(&tick_start, NULL);

	// LOGGING
	n++;
	fprintf(stderr, "============= TICK %ld =============\n", n);
	teams_log(env, true);
	teams_log(env, false);
	check_game_start(env);
	if ((code = receipt(env)) != ERR_NONE
		|| 	(env->start && (code = update_players(env))) != ERR_NONE
		|| 	(code = update_eggs(env)) != ERR_NONE
		|| (code = update_commands(env)) != ERR_NONE)
		return (code);

	gettimeofday(&tick_end, NULL);
	elapsed = tick_end.tv_usec - tick_start.tv_usec;
	usleep(env->settings.tick_length - elapsed);
	fprintf(stderr, "Waiting %d microseconds before the start of the next cycle...\n", env->settings.tick_length - elapsed);
	return (ERR_NONE); //Leaks test
}


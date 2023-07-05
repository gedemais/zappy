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
}

static void	check_game_end(t_env *env)
{
	t_team		*team;
	t_player	*player;
	bool		won;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		won = true;
		team = dyacc(&env->world.teams, i);
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
	uint8_t			code;
	struct timeval	tick_start, tick_end;
	useconds_t		elapsed;

	gettimeofday(&tick_start, NULL);

	teams_log(env);
	env->start ? check_game_end(env) : check_game_start(env);

	if ((code = receipt(env)) != ERR_NONE
		|| 	(env->start && (code = update_players(env))) != ERR_NONE
		|| 	(code = update_eggs(env)) != ERR_NONE
		|| (code = update_commands(env)) != ERR_NONE)
		return (code);
	//send_responses

	// Decrement players satiety and kill them if at 0 (send "mort" to client)
	// Check for disappeared food, if so then generate the same amount
	// Check for a winner team
	// Execute clients commands and send responses with a limited time (one tick).
	// Wait for elapsed time
	//sleep(1);
// tmp
	//t_team *a = dyacc(&env->world.teams, 0);
	//t_player *pa = dyacc(&a->players, 0);
	//write(1, "-> ", 3);
	//get_next_line(0, &request);
// tmp
	//if ((code = launch_command(env, pa, request)) != ERR_NONE)
	//{
	//	free(request);
//		return (code);
//	}

//	free(request);

	gettimeofday(&tick_end, NULL);
	elapsed = tick_end.tv_usec - tick_start.tv_usec;
	usleep(env->settings.tick_length - elapsed);
	return (ERR_NONE); //Leaks test
}


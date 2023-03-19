#include "main.h"

static uint8_t	update_players(t_env *env)
{
	t_team		*t;
	t_player	*p;

	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);
			if (p->alive == true)
				update_food(p);
		}
	}

	//teams_log(env);

	return (ERR_NONE);
}

uint8_t	tick(t_env *env)
{
	uint8_t			code;
	struct timeval	tick_start, tick_end;
	useconds_t		elapsed;

	gettimeofday(&tick_start, NULL);

	if ((code = update_players(env)) != ERR_NONE
		|| (code = update_commands(env)) != ERR_NONE
		|| (code = receipt(env)) != ERR_NONE)
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
	return (ERR_NONE);
}


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

			//check_for_command(env, p);
		}
	}

	teams_log(env);

	return (ERR_NONE);
}

uint8_t	tick(t_env *env)
{
	update_players(env);
	// Decrement players satiety and kill them if at 0 (send "mort" to client)
	// Check for disappeared loot, if so then generate the same amount
	// Check for a winner team
	// Execute clients commands and send responses with a limited time (one tick).
	// Wait for elapsed time
	//sleep(1);
	return (ERR_NONE);
}

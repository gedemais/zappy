#include "main.h"

static void	free_teams(t_env *env)
{
	t_team	*t;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		free(t->name);
		free_dynarray(&t->players);
	}
	free_dynarray(&env->world.teams);
}

void		free_env(t_env *env)
{
	free_teams(env);
}

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

static void	free_world(t_env *env)
{
	for (uint32_t i = 0; i < env->settings.map_height; i++)
	{
		for (uint32_t j = 0; j < env->settings.map_width; j++)
			if (env->world.map[i][j].content.byte_size > 0)
				free_dynarray(&env->world.map[i][j].content);

		free(env->world.map[i]);
	}
	free(env->world.map);
}

void		free_env(t_env *env)
{
	free(env->response);
	free_teams(env);
	free_world(env);
}

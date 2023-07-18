#include "main.h"

static void	free_teams(t_env *env)
{
	t_team		*t;
	t_player	*player;

	// For each team
	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		if (t->name)
			free(t->name);
		for (int p = 0; p < t->players.nb_cells; p++)
		{
			player = dyacc(&t->players, p);
			dynarray_free(&player->cmd_queue);
		}
		dynarray_free(&t->players);
	}
	dynarray_free(&env->world.teams);

	// Pending team
	dynarray_free(&env->world.pending.players);
}

static void	free_world(t_env *env)
{
	if (env->world.map == NULL)
		return ;

	for (uint32_t i = 0; i < env->settings.map_height; i++)
		free(env->world.map[i]);
	free(env->world.map);
}

static void	free_buffers(t_env *env)
{
	free(env->buffers.response);
	free(env->buffers.request);
	free(env->buffers.connections);
	dynarray_free(&env->buffers.view);
}

void		free_cmd(t_cmd *cmd)
{
	if (cmd->tokens)
		ft_arrfree(cmd->tokens);
}

void		free_env(t_env *env)
{
	free_buffers(env);
	free_teams(env);
	free_world(env);
}

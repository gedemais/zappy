#include "main.h"

static void	free_teams(t_env *env)
{
	t_team	*t;

	// For each team
	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		if (t->name)
			free(t->name);
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
	if (cmd->response)
		free(cmd->response);
}

static void	free_commands_queue(t_env *env)
{
	t_dynarray	*cmd_queue;
	t_cmd		*cmd;

	cmd_queue = &env->buffers.cmd_queue;
	for (int i = 0; i < cmd_queue->nb_cells; i++)
	{
		cmd = dyacc(cmd_queue, i);
		free_cmd(cmd);
	}
	dynarray_free(cmd_queue);
}

void		free_env(t_env *env)
{
	free_buffers(env);
	free_teams(env);
	free_world(env);
	free_commands_queue(env);
}

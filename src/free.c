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

static void	free_buffers(t_env *env)
{
	free(env->buffers.response);
	free(env->buffers.request);
	free(env->buffers.connections);
	free_dynarray(&env->buffers.view);
}

void		free_cmd(t_cmd *cmd)
{
	if (cmd->tokens)
		ft_free_ctab(cmd->tokens);
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
}

void		free_env(t_env *env)
{
	free_buffers(env);
	free_teams(env);
	free_world(env);
	free_commands_queue(env);
}

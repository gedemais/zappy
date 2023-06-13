#include "main.h"

static uint8_t	run_command(t_env *env, t_player *p, t_cmd *cmd)
{
	uint8_t	code;

	env->buffers.cmd_params = &cmd->tokens[1];

	if ((code = cmd->cmd_func(env, p, true)) != ERR_NONE)
		return (code);

	free_cmd(cmd);
	dynarray_pop(&p->cmd_queue, true);
	return (ERR_NONE);
}

static uint8_t handle_command(t_env *env, t_player *p, t_cmd *cmd)
{
	if (cmd->cycles > 0)
		cmd->cycles--;
	else
		return (run_command(env, p, cmd));
	return (ERR_NONE);
}

// This function will process command queue by executing commands
uint8_t	update_commands(t_env *env)
{
	t_team		*team;
	t_player	*p;
	uint8_t		code;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < team->players.nb_cells; j++)
		{
			p = dyacc(&team->players, j);
			if (p->cmd_queue.nb_cells > 0 && (code = handle_command(env, p, dyacc(&p->cmd_queue, 0))))
				return (code);
				
		}
	}
	return (ERR_NONE);
}

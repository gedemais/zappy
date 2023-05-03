#include "main.h"

static uint8_t	run_command(t_env *env, t_dynarray *cmd_queue, t_cmd *cmd, int i)
{
	uint8_t	code;

	if ((code = cmd->cmd_func(env, cmd->p, true)))
		return (code);
	else
	{
		free_cmd(dyacc(cmd_queue, i));
		if (extract_dynarray(cmd_queue, i))
			return (ERR_MALLOC_FAILED);
	}

	return (ERR_NONE);
}

// This function will process command queue by executing commands
uint8_t	update_commands(t_env *env)
{
	t_cmd		*cmd;
	t_dynarray	*cmd_queue;
	uint8_t		code;
	int			i = 0;

	//tmp
	t_player	*p;
	t_team		*t;


	t = &env->world.pending;
	p = dyacc(&t->players, 0);
	// tmp

	cmd_queue = &env->buffers.cmd_queue;
	// Iterate on cmd queue
	while (i < cmd_queue->nb_cells)
	{
		cmd = dyacc(cmd_queue, i);

		// Cycles waiting mechanism
		if (cmd->cycles > 0)
			cmd->cycles--;
		else
		{
			// When time has come, we execute the command by the player who sent it
			if ((code = run_command(env, cmd_queue, cmd, i)))
				return (code);
			continue;
		}
	
		i++;
	}
	return (ERR_NONE);
}

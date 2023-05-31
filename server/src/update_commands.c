#include "main.h"

static uint8_t	run_command(t_env *env, t_dynarray *cmd_queue, t_cmd *cmd, int i)
{
	uint8_t	code;

	env->buffers.cmd_params = &cmd->tokens[1];
	code = cmd->cmd_func(env, cmd->p, true);

	free_cmd(dyacc(cmd_queue, i));
	if (dynarray_extract(cmd_queue, i))
		return (ERR_MALLOC_FAILED);

	return (code);
}

// This function will process command queue by executing commands
uint8_t	update_commands(t_env *env)
{
	t_cmd		*cmd;
	t_dynarray	*cmd_queue;
	uint8_t		code;
	int			i = 0;

	cmd_queue = &env->buffers.cmd_queue;
	printf("%d\n", cmd_queue->nb_cells);
	fflush(stdout);
	// Iterate on cmd queue
	while (i < cmd_queue->nb_cells)
	{
		cmd = dyacc(cmd_queue, i);

		// Cycles waiting mechanism
		if (cmd->cycles > 0)
			cmd->cycles--;
		else
		{
			printf("%d\n", i);
			// When time has come, we execute the command by the player who sent it
			cmd->p->queued_commands--;
			if ((code = run_command(env, cmd_queue, cmd, i)) != ERR_NONE)
				return (code);
			continue;
		}
		i++;
	}
	printf("%d\n", cmd_queue->nb_cells);
	return (ERR_NONE);
}

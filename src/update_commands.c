#include "main.h"

static uint8_t	run_command(t_env *env, t_dynarray *cmd_queue, t_cmd *cmd, t_player *p, int i)
{
	uint8_t	code;

	if ((code = cmd->cmd_func(env, p, true)))
		return (code);
	else
	{
		free_cmd(dyacc(cmd_queue, i));
		if (extract_dynarray(cmd_queue, i))
			return (ERR_MALLOC_FAILED);
	}

	return (ERR_NONE);
}

uint8_t	update_commands(t_env *env)
{
	t_cmd		*cmd;
	t_dynarray	*cmd_queue;
	uint8_t		code;
	int			i = 0;

	//tmp
	t_player	*p;
	t_team		*t;


	t = dyacc(&env->world.teams, 0);
	p = dyacc(&t->players, 0);
	// tmp

	cmd_queue = &env->buffers.cmd_queue;
	while (i < cmd_queue->nb_cells)
	{
		cmd = dyacc(cmd_queue, i);

		if (cmd->cycles > 0)
			cmd->cycles--;
		else
		{
			if ((code = run_command(env, cmd_queue, cmd, p, i))) // p to remove later as it will be stored where cmd points
				return (code);
			continue;
		}
	
		i++;
	}
	return (ERR_NONE);
}

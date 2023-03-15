#include "main.h"

uint8_t	build_message_from_params(t_env *env)
{
	uint16_t	i = 0;

	FLUSH_RESPONSE
	while (env->buffers.cmd_params[i])
	{
		strcat(env->buffers.response, env->buffers.cmd_params[i]);
		strcat(env->buffers.response, env->buffers.cmd_params[i + 1] ? " " : "");
		i++;
	}
}

uint8_t	deliver_message(t_env *env)
{
	t_team		*te;
	t_player	*pl;

	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		te = dyacc(&env->world.teams, team);
		for (int player = 0; player < te->players.nb_cells; player++)
		{
			pl = dyacc(&te->players, player);
			if (pl == p)
				continue;
			// Compute shortest path + direction and add it to response
			response(env, pl);
		}
	}
	return (ERR_NONE);
}

#include "main.h"

uint8_t	send_graphical_data(t_env *env)
{
	t_team		*team;
	t_egg		*egg;
	t_player	*pl;
	uint8_t		code;
	char		buffer[128];

	FLUSH_GRESPONSE

	if ((code = gcmd_map_size(env)) != ERR_NONE
		|| (code = gcmd_server_time_unit(env)) != ERR_NONE
		|| (code = gcmd_map_content(env)) != ERR_NONE
		|| (code = gcmd_teams_names(env)) != ERR_NONE)
		return (code);

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < team->players.nb_cells; j++)
		{
			pl = dyacc(&team->players, j);
			memcpy(&env->gplayer, pl, sizeof(t_player));
			gcmd_player_new(env);
		}
	}

	for (int e = 0; e < env->world.eggs.nb_cells; e++)
	{
		egg = dyacc(&env->world.eggs, e);
		env->gindex = egg->id;
		env->gx = egg->x;
		env->gy = egg->y;
		env->gpid = egg->pid;
		bzero(buffer, 128);
		sprintf(buffer, "enw #%d #%d %d %d\n", egg->id, egg->pid, egg->x, egg->y);
		strcat(env->buffers.gresponse, buffer);
	}

	return (gresponse(env));
}

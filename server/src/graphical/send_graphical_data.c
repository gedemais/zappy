#include "main.h"

uint8_t	send_graphical_data(t_env *env, t_player *p)
{
	t_team		*team;
	t_player	*pl;
	uint8_t		code;

	FLUSH_RESPONSE

	if ((code = gcmd_map_size(env))
		|| (code = gcmd_server_time_unit(env)))
		return (code);

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < env->world.teams.nb_cells; j++)
		{
			pl = dyacc(&team->players, j);
			env->gplayer = pl;
			gcmd_player_new(env);
		}
	}

	/*for (int e = 0; e < env->world.eggs.nb_cells; e++)
	{
		env->gindex = e;
		gcmd_egg_new(env);
	}*/


	return (response(env, p));
}

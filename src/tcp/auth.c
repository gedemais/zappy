#include "main.h"

uint8_t	auth(t_env *env)
{
	t_team		*t;
	t_player	*p;
	int			team_index = 0;
	int			i = 0;

	while (i < env->world.pending.players.nb_cells)
	{
		p = dyacc(&env->world.pending.players, i);
		if (p->auth_step == AS_MAX)
		{
			// Remove player from pending team and put it in its new team
			team_index = 0;
			t = dyacc(&env->world.teams, team_index);
			if (push_dynarray(&t->players, p, false))
				return (ERR_MALLOC_FAILED);
			extract_dynarray(&env->world.pending.players, i);
			i = 0;
		}
		i++;
	}
	return (ERR_NONE);
}

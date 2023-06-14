#include "main.h"

uint8_t	gevent_player_new(t_env *env)
{
	t_team	*team;
	char	s[128];

	team = dyacc(&env->world.teams, env->gplayer->team);
	sprintf(s, "pnw #%d %d %d %d %d %s\n",	env->gplayer->pid,
			env->gplayer->tile_x,
			env->gplayer->tile_y,
			(int)env->gplayer->direction.d,
			env->gplayer->level,
			team->name);

	strcat(env->buffers.gresponse, s);
	return (ERR_NONE);
}


uint8_t	gevent_player_expulse(t_env *env)
{
	char	s[128];

	FLUSH_GRESPONSE
	sprintf(s, "pex #%d\n", env->gplayer->pid);

	strcat(env->buffers.gresponse, s);
	return (gresponse(env, &env->graphical));
}

uint8_t	gevent_player_broadcast(t_env *env)
{
	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pbc #%d %s\n", env->gplayer->pid, &env->buffers.response[10]);
	return (gresponse(env, &env->graphical));
}

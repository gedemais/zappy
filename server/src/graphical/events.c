#include "main.h"

uint8_t	gevent_player_new(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	t_team	*team;
	char	s[128];

	if (env->graphical.team == 0)
		return (ERR_NONE);

	team = dyacc(&env->world.teams, env->gplayer.team);
	sprintf(s, "pnw #%d %d %d %d %d %s\n",	env->gplayer.pid,
			env->gplayer.tile_x,
			env->gplayer.tile_y,
			(int)env->gplayer.direction.d,
			env->gplayer.level,
			team->name);

	strcat(env->buffers.gresponse, s);
	return (gresponse(env));
}

uint8_t	gevent_player_expulse(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	char	s[128];

	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(s, "pex #%d\n", env->gplayer.pid);

	strcat(env->buffers.gresponse, s);
	return (gresponse(env));
}

uint8_t	gevent_player_broadcast(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pbc #%d %s\n", env->gplayer.pid, env->gstr);
	return (gresponse(env));
}

uint8_t	gevent_player_lays_egg(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pfk #%d\n", env->gplayer.pid);
	return (gresponse(env));
}

uint8_t	gevent_player_put(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pdr #%d %d\n", env->gplayer.pid, env->gindex);
	return (gresponse(env));
}

uint8_t	gevent_player_take(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pgt #%d %d\n", env->gplayer.pid, env->gindex);
	return (gresponse(env));
}

uint8_t	gevent_player_died(t_env *env)
{
	if (env->graphical.team == 0
		|| env->graphical.connection == env->gplayer.connection)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pdi #%d\n", env->gplayer.pid);
	return (gresponse(env));
}

uint8_t	gevent_player_layed_egg(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "enw #%d #%d %d %d\n", env->gindex, env->gpid, env->gx, env->gy);
	return (gresponse(env));
}

uint8_t	gevent_egg_hatched(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "eht #%d\n", env->gindex);
	return (gresponse(env));
}

uint8_t	gevent_player_connected_for_egg(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "ebo #%d", env->gindex);
	return (gresponse(env));
}

uint8_t	gevent_egg_rotted(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "edi #%d\n", env->gindex);
	return (gresponse(env));
}

uint8_t	gevent_game_ended(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	t_team	*team;

	team = dyacc(&env->world.teams, env->gindex);
	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "seg %s\n", team->name);
	return (gresponse(env));
}

uint8_t	gevent_broadcast(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "smg %s\n", env->buffers.response);
	return (gresponse(env));
}

uint8_t		gevent_incantation_launch(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	t_team			*t;
	t_player		*p;
	t_player		*pl;
	char			tmp[128];

	p = &env->gplayer;
	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pic %d %d %d ", p->tile_x, p->tile_y, p->level);

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		for (int j = 0; j < env->world.teams.nb_cells; j++)
		{
			pl = dyacc(&t->players, j);
			bool coords = (bool)(pl->tile_x == p->tile_x && pl->tile_y == p->tile_y);
			if (coords && pl->level == p->level)
			{
				bzero(tmp, sizeof(char) * 128);
				sprintf(tmp, "#%d ", p->pid);
				strcat(env->buffers.gresponse, tmp);
			}
		}
	}

	env->buffers.gresponse[strlen(env->buffers.gresponse) - 1] = '\n';

	return (gresponse(env));
}

uint8_t	gevent_incantation_ended(t_env *env)
{
	if (env->graphical.team == 0)
		return (ERR_NONE);

	t_player	*p;

	p = &env->gplayer;
	FLUSH_GRESPONSE
	sprintf(env->buffers.gresponse, "pie %d %d %d\n", p->tile_x, p->tile_y, 1);
	return (gresponse(env));
}

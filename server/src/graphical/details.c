#include "main.h"

uint8_t	gcmd_map_size(t_env *env)
{
	char	*w, *h;

	if (!(w = ft_itoa(env->settings.map_width)))
		return (ERR_MALLOC_FAILED);

	if (!(h = ft_itoa(env->settings.map_height)))
	{
		free(w);
		return (ERR_MALLOC_FAILED);
	}

	strcat(env->buffers.response, "msz ");
	strcat(env->buffers.response, w);
	free(w);
	strcat(env->buffers.response, " ");
	strcat(env->buffers.response, h);
	free(h);
	strcat(env->buffers.response, "\n");
	return (ERR_NONE);
}

uint8_t	gcmd_server_time_unit(t_env *env)
{
	char	*t;

	if (!(t = ft_itoa(env->settings.t)))
		return (ERR_MALLOC_FAILED);

	strcat(env->buffers.response, "sgt ");
	strcat(env->buffers.response, t);
	free(t);
	strcat(env->buffers.response, "\n");
	return (ERR_NONE);
}

uint8_t	gcmd_block_content(t_env *env)
{
	strcat(env->buffers.response, "bct ");
	cat_spaced_number(env, env->gx, false);
	cat_spaced_number(env, env->gy, false);

	for (unsigned int i = 0; i < LOOT_MAX; i++)
		cat_spaced_number(env, env->world.map[env->gy][env->gx].content[i], i == LOOT_MAX - 1);

	env->buffers.response[strlen(env->buffers.response) - 1] = '\n';

	return (ERR_NONE);
}

uint8_t	gcmd_teams_names(t_env *env)
{
	t_team	*team;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		strcat(env->buffers.response, "tna ");
		strcat(env->buffers.response, team->name);
		strcat(env->buffers.response, "\n");
	}
	return (ERR_NONE);
}

uint8_t	gcmd_player_new(t_env *env)
{
	t_team	*team;

	strcat(env->buffers.response, "pnw #");
	cat_spaced_number(env, env->gplayer->pid, false);
	cat_spaced_number(env, env->gplayer->tile_x, false);
	cat_spaced_number(env, env->gplayer->tile_y, false);
	cat_spaced_number(env, (int)env->gplayer->direction.d, false);
	cat_spaced_number(env, env->gplayer->level, false);

	team = dyacc(&env->world.teams, env->gplayer->team);

	strcat(env->buffers.response, team->name);
	strcat(env->buffers.response, "\n");

	return (ERR_NONE);
}

uint8_t	gcmd_egg_new(t_env *env)
{
	t_egg	*egg;

	return (ERR_NONE);
}

/*
uint8_t	gcmd_(t_env *env)
{

	return (ERR_NONE);
}*/

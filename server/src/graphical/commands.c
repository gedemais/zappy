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
	char	s[128];

	team = dyacc(&env->world.teams, env->gplayer.team);
	sprintf(s, "pnw #%d %d %d %d %d %s\n",	env->gplayer.pid,
			env->gplayer.tile_x,
			env->gplayer.tile_y,
			(int)env->gplayer.direction.d,
			env->gplayer.level,
			team->name);

	strcat(env->buffers.response, s);
	return (ERR_NONE);
}

uint8_t	gcmd_map_content(t_env *env)
{
	for (unsigned int y = 0; y < env->settings.map_height; y++)
		for (unsigned int x = 0; x < env->settings.map_height; x++)
		{
			env->gx = x;
			env->gy = y;
			gcmd_block_content(env);
		}
	return (ERR_NONE);
}

uint8_t	gcmd_player_position(t_env *env)
{
	char	s[128];

	sprintf(s, "ppo #%d %d %d %d\n",	env->gplayer.pid,
			env->gplayer.tile_x,
			env->gplayer.tile_y,
			(int)env->gplayer.direction.d);

	strcat(env->buffers.response, s);
	return (ERR_NONE);
}


uint8_t	gcmd_player_level(t_env *env)
{
	char	s[128];

	sprintf(s, "plv #%d %d\n", env->gplayer.pid, env->gplayer.level);
	strcat(env->buffers.response, s);
	return (ERR_NONE);
}


uint8_t	gcmd_player_inventory(t_env *env)
{
	char	s[128];

	sprintf(s, "pin #%d %d %d ", env->gplayer.pid,
			env->gplayer.tile_x,
			env->gplayer.tile_y);

	strcat(env->buffers.response, s);

	for (int i = 0; i < LOOT_MAX; i++)
		cat_spaced_number(env, env->gplayer.inventory[i], i == LOOT_MAX - 1);

	strcat(env->buffers.response, "\n");
	return (ERR_NONE);
}


uint8_t	gcmd_set_new_t(t_env *env)
{
	char	s[128];

	env->settings.t = env->gnew_t;
	env->settings.tick_length = 1000000 / env->settings.t;

	sprintf(s, "sgt %d\n", env->settings.t);
	strcat(env->buffers.response, s);

	return (ERR_NONE);
}

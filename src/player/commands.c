#include "main.h"

uint8_t	cmd_see(t_env *env, t_player *p)
{
	t_view_ranges	ranges;
	int16_t			tx, ty;

	for (uint8_t i = 0; i < p->level; i++)
	{
		compute_view_ranges(env, &ranges, p, i);
		for (int16_t x = min(ranges.start_x, ranges.end_x); x <= max(ranges.start_x, ranges.end_x); x++)
			for (int16_t y = min(ranges.start_y, ranges.end_y); y <= max(ranges.start_y, ranges.end_y); y++)
			{
				tx = x;
				ty = y;
				clamp(&tx, 0, env->settings.map_width);
				clamp(&ty, 0, env->settings.map_height);
				if (push_dynarray(&env->buffers.view, &env->world.map[ty][tx], false))
				{
					clear_dynarray(&env->buffers.view);
					return (ERR_MALLOC_FAILED);
				}
			}
	}
	send_see_response(env, &env->buffers.view);
	clear_dynarray(&env->buffers.view);
	return (ERR_NONE);
}

uint8_t	cmd_inventory(t_env *env, t_player *p)
{
	char	*amnt;

	ft_strcat(env->buffers.response, "{");
	for (uint16_t i = 0; i < LOOT_MAX; i++)
	{
		if (!(amnt = ft_itoa((int)p->inventory[i])))
			return (ERR_MALLOC_FAILED);

		ft_strcat(env->buffers.response, amnt);
		free(amnt);

		ft_strcat(env->buffers.response, " ");
		ft_strcat(env->buffers.response, loot_titles[i]);

		if (i < LOOT_MAX - 1)
			ft_strcat(env->buffers.response, ", ");
	}
	ft_strcat(env->buffers.response, "}");
	printf("%s\n", env->buffers.response);
	memset(env->buffers.response, 0, strlen(env->buffers.response));
	return (ERR_NONE);
}

uint8_t	cmd_take(t_env *env, t_player *p)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	*loot_ptr;
	uint8_t	loot = 255;
	bool	found = false;

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_param, loot_titles[i]) == 0)
			loot = i;

	for (int i = 0; i < tile->content.nb_cells; i++)
	{
		loot_ptr = dyacc(&tile->content, i);
		if ((uint8_t)*loot_ptr == loot && (found = true)
			&& extract_dynarray(&tile->content, i))
			return (ERR_MALLOC_FAILED);
	}

	if (loot == 255 || tile->content.nb_cells == 0 || found == false)
		ft_strcat(env->buffers.response, "ko");

	else
	{
		p->inventory[(int)*loot_ptr]++;
		memset(env->buffers.response, 0, strlen(env->buffers.response));
		ft_strcat(env->buffers.response, "ok");
		printf("%s\n", env->buffers.response);
	}
	return (ERR_NONE);
}

uint8_t	cmd_put(t_env *env, t_player *p)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_param, loot_titles[i]) == 0)
			loot = i;
	
	if (loot == 255 || p->inventory[loot] == 0)
		ft_strcat(env->buffers.response, "ko");

	p->inventory[loot]--;
	if ((tile->content.byte_size == 0 && init_dynarray(&tile->content, sizeof(uint8_t), 4))
		|| push_dynarray(&tile->content, &loot, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

/*
uint8_t	cmd_kick(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}
*/

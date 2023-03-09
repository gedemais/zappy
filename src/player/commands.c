#include "main.h"

uint8_t	cmd_advance(t_env *env, t_player *p)
{
	const int8_t	moves[DIR_MAX][2] ={{0, -1},
										{1, 0},
										{0, 1},
										{-1, 0}};

	for (int dir = 0; dir < DIR_MAX; dir++)
		if (p->direction.d == dir)
		{
			p->tile_x += moves[dir][0];
			p->tile_y += moves[dir][1];


			p->tile_x = p->tile_x >= env->settings.map_width ? 0 : p->tile_x;
			p->tile_y = p->tile_y >= env->settings.map_height ? 0 : p->tile_y;

			p->tile_x = p->tile_x < 0 ? env->settings.map_width - 1 : p->tile_x;
			p->tile_y = p->tile_y < 0 ? env->settings.map_height - 1 : p->tile_y;
			return (ERR_NONE);
		}

	return (ERR_NONE);
}

uint8_t	cmd_left(t_env *env, t_player *p)
{
	(void)env;
	p->direction.d--;
	return (ERR_NONE);
}

uint8_t	cmd_right(t_env *env, t_player *p)
{
	(void)env;
	p->direction.d++;
	return (ERR_NONE);
}

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

/*
uint8_t	cmd_(t_env *env, t_player *p)
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

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}
*/

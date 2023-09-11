#include "main.h"

uint8_t	view_y(t_env *env, t_player *p)
{
	int16_t	x, tx, y, ty;
	bool	north;

	north = (bool)(p->direction.d == DIR_NORTH);
	for (int l = 1; l <= p->level; l++)
	{
		y = north ? -l : l;
		for (int x = -l; x <= l; x++)
		{
			tx = p->tile_x + (north ? x : -x);
			ty = p->tile_y + y;

			clamp(&tx, 0, env->settings.map_width);
			clamp(&ty, 0, env->settings.map_height);

			env->world.map[ty][tx].content[LOOT_VIEWED] = 1;

			if (dynarray_push(&env->buffers.view, &env->world.map[ty][tx], false))
				return (ERR_MALLOC_FAILED);
		}
	}
	return (ERR_NONE);
}

uint8_t	view_x(t_env *env, t_player *p)
{
	int16_t	x, tx, y, ty;
	bool	east;

	east = (bool)(p->direction.d == DIR_EAST);
	for (int l = 1; l <= p->level; l++)
	{
		x = east ? l : -l;
		for (int y = -l; y <= l; y++)
		{
			tx = p->tile_x + x;
			ty = p->tile_y + (east ? y : -y);

			clamp(&tx, 0, env->settings.map_width);
			clamp(&ty, 0, env->settings.map_height);

			env->world.map[ty][tx].content[LOOT_VIEWED] = 1;

			if (dynarray_push(&env->buffers.view, &env->world.map[ty][tx], false))
				return (ERR_MALLOC_FAILED);
		}
	}
	return (ERR_NONE);
}

void	send_see_response(t_env *env, t_dynarray *view, t_player *p)
{
	t_tile	*tile;

	FLUSH_RESPONSE
	strcat(env->buffers.response, "{");
	for (int i = 0; i < view->nb_cells; i++)
	{
		tile = dyacc(view, i);
		strcat(env->buffers.response, i > 0 ? ", " : "");
		for (int j = 0; j <= LOOT_PLAYER; j++)
		{
			for (uint16_t k = 0; k < tile->content[j]; k++)
			{
				strcat(env->buffers.response, loot_titles[j]);
				printf("tile_content[%d] : %d\n", j, tile->content[j]);
				printf("len: %ld\n", strlen(env->buffers.response));
				strcat(env->buffers.response, " ");
			}
		}
	}
	strcat(env->buffers.response, "}\n");
	response(env, p);
}

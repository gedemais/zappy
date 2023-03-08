#include "main.h"

const char	*loot_titles[LOOT_MAX] = {
	[LOOT_LINEMATE] = "linemate",
	[LOOT_DERAUMERE] = "deraumere",
	[LOOT_SIBUR] = "sibur",
	[LOOT_MENDIANE] = "mendiane",
	[LOOT_PHIRAS] = "phiras",
	[LOOT_THYSTAME] = "thystame",
	[LOOT_FOOD] = "nourriture"
};


void	free_view(t_dynarray *view)
{
	for (int i = 0; i < view->nb_cells; i++)
		free_dynarray(dyacc(view, i));
	free_dynarray(view);
}

void	send_see_response(t_env *env, t_dynarray *view)
{
	t_dynarray	*tile;
	uint8_t		*loot;

	ft_strcat(env->response, "{");
	for (int i = 0; i < view->nb_cells; i++)
	{
		tile = dyacc(view, i);
		ft_strcat(env->response, (i > 0 && i < view->nb_cells) ? ", " : "");
		for (int j = 0; j < tile->nb_cells; j++)
		{
			loot = dyacc(tile, j);
			ft_strcat(env->response, (j > 0 && j < tile->nb_cells - 1) ? " " : "");
			if ((int)*loot != 9)
				ft_strcat(env->response, loot_titles[(int)*loot]);
		}
	}
	ft_strcat(env->response, "}");
	printf("%s\n", env->response);
}

void	compute_view_ranges(t_env *env, t_view_ranges *ranges, t_player *p, uint8_t i)
{
	const int8_t	moves[DIR_MAX][2] = {[DIR_NORTH] = {0, -1},
										 [DIR_EAST] = {1, 0},
										 [DIR_SOUTH] = {0, 1},
										 [DIR_WEST] = {-1, 0}};

	ranges->middle_x = p->tile_x + moves[p->direction.d][0] * i;
	ranges->middle_y = p->tile_y + moves[p->direction.d][1] * i;

	clamp(&ranges->middle_x, 0, env->settings.map_width);
	clamp(&ranges->middle_y, 0, env->settings.map_height);

	p->direction.d -= 1;
	ranges->start_x = ranges->middle_x + moves[p->direction.d][0] * i;
	ranges->start_y = ranges->middle_y + moves[p->direction.d][1] * i;
	p->direction.d += 2;
	ranges->end_x = ranges->middle_x + moves[p->direction.d][0] * i;
	ranges->end_y = ranges->middle_y + moves[p->direction.d][1] * i;
	p->direction.d -= 1;
}

uint8_t		add_tile_to_view(t_dynarray *view, t_dynarray *tile_content)
{
	t_dynarray	tile_loot;
	uint8_t		*loot;


	if (init_dynarray(&tile_loot, sizeof(uint8_t), tile_content->nb_cells))
		return (ERR_MALLOC_FAILED);

	if (tile_content->byte_size > 0)
		for (int i = 0; i < tile_content->nb_cells; i++)
		{
			loot = dyacc(tile_content, i);
			if (push_dynarray(&tile_loot, loot, false))
				return (ERR_MALLOC_FAILED);
		}

	if (push_dynarray(view, &tile_loot, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

#include "main.h"

static int find_player_index_in_tile(t_tile *tile)
{
	uint8_t	*loot;

	for (int i = 0; i < tile->content.nb_cells; i++)
	{
		loot = dyacc(&tile->content, i);
		if ((int)*loot == 255)
			return (i);
	}
	return (-1);
}

uint8_t		cmd_advance(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	system("clear");
	t_tile	*tile;
	uint8_t	loot = 255;

	for (int dir = 0; dir < DIR_MAX; dir++)
		if (p->direction.d == dir)
		{
			tile = &env->world.map[p->tile_y][p->tile_x];
			printf("%d\n", tile->content.nb_cells);
			if (extract_dynarray(&tile->content, find_player_index_in_tile(tile)))
				return (ERR_MALLOC_FAILED);
			printf("%d\n----------\n", tile->content.nb_cells);

			p->tile_x += moves[dir][0];
			p->tile_y += moves[dir][1];

			p->tile_x = p->tile_x >= env->settings.map_width ? 0 : p->tile_x;
			p->tile_y = p->tile_y >= env->settings.map_height ? 0 : p->tile_y;

			p->tile_x = p->tile_x < 0 ? env->settings.map_width - 1 : p->tile_x;
			p->tile_y = p->tile_y < 0 ? env->settings.map_height - 1 : p->tile_y;

			tile = &env->world.map[p->tile_y][p->tile_x];

			if ((tile->content.byte_size == 0
				&& init_dynarray(&tile->content, sizeof(uint8_t), 4))
				|| push_dynarray(&tile->content, &loot, false))
				return (ERR_MALLOC_FAILED);
		}
	
	return (ERR_NONE);
}

uint8_t	cmd_left(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	system("clear");
	(void)env;
	p->direction.d--;
	return (ERR_NONE);
}

uint8_t	cmd_right(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	system("clear");
	(void)env;
	p->direction.d++;
	return (ERR_NONE);
}


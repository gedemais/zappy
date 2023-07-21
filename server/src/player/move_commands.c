#include "main.h"

uint8_t		cmd_advance(t_env *env, t_player *p, bool send_response)
{
	t_tile	*tile;

	for (int dir = 0; dir < DIR_MAX; dir++)
		if (p->direction.d == dir)
		{
			if (remove_player_from_tile(env, p->tile_x, p->tile_y))
				return (ERR_MALLOC_FAILED);

			p->tile_x += moves[dir][0];
			p->tile_y += moves[dir][1];

			p->tile_x = p->tile_x >= env->settings.map_width ? 0 : p->tile_x;
			p->tile_y = p->tile_y >= env->settings.map_height ? 0 : p->tile_y;

			p->tile_x = p->tile_x < 0 ? env->settings.map_width - 1 : p->tile_x;
			p->tile_y = p->tile_y < 0 ? env->settings.map_height - 1 : p->tile_y;

			tile = &env->world.map[p->tile_y][p->tile_x];

			tile->content[LOOT_PLAYER]++;

			return (send_response ? send_ok(env, p) : ERR_NONE);
		}
	
	return (ERR_NONE);
}

uint8_t	cmd_left(t_env *env, t_player *p, bool send_response)
{
	p->direction.d++;
	return (send_response ? send_ok(env, p) : ERR_NONE);
}

uint8_t	cmd_right(t_env *env, t_player *p, bool send_response)
{
	p->direction.d--;
	return (send_response ? send_ok(env, p) : ERR_NONE);
}


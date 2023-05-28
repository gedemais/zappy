#include "main.h"

uint8_t		cmd_advance(t_env *env, t_player *p, bool send_response)
{
	printf("%s\n", __FUNCTION__);
	t_tile	*tile;
	uint8_t	loot = 255;

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


			if (tile->content.byte_size == 0 && init_dynarray(&tile->content, sizeof(uint8_t), 4))
			{
				printf("init\n");
				fflush(stdout);
				return (ERR_MALLOC_FAILED);
			}

			if (push_dynarray(&tile->content, &loot, false))
			{
				printf("push\n");
				fflush(stdout);
				return (ERR_MALLOC_FAILED);
			}

			if (send_response)
			{
				FLUSH_RESPONSE
				strcat(env->buffers.response, "ok\n");
				response(env, p);
			}
		}
	
	printf("np\n");
	return (ERR_NONE);
}

uint8_t	cmd_left(t_env *env, t_player *p, bool send_response)
{
	printf("%s\n", __FUNCTION__);
	p->direction.d--;
	if (send_response)
	{
		FLUSH_RESPONSE
		strcat(env->buffers.response, "ok\n");
		response(env, p);
	}
	return (ERR_NONE);
}

uint8_t	cmd_right(t_env *env, t_player *p, bool send_response)
{
	printf("%s\n", __FUNCTION__);
	p->direction.d++;
	if (send_response)
	{
		FLUSH_RESPONSE
		strcat(env->buffers.response, "ok\n");
		response(env, p);
	}
	return (ERR_NONE);
}


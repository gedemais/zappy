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
	// todo return (ERR_INVALID_PLAYER_DIRECTION);
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

static void		clamp(int16_t *v, int16_t min, int16_t max)
{
	if (*v < min)
		*v = max - (min - *v);
	else if (*v >= max)
		*v = min + (*v - max);
}

uint8_t	cmd_see(t_env *env, t_player *p)
{
	//char		*response;
	int16_t		middle_x, middle_y, start_x, start_y, end_x, end_y;
	int8_t		moves[DIR_MAX][2] ={{0, -1},
								{1, 0},
								{0, 1},
								{-1, 0}};

	
	//p->tile_x - (int)floor(fov_width[i] / 2.0f);
	// add_tile_to_view

	for (uint8_t i = 0; i <= p->level; i++)
	{
		middle_x = p->tile_x + moves[p->direction.d][0] * i;
		middle_y = p->tile_y + moves[p->direction.d][1] * i;

		clamp(&middle_x, 0, env->settings.map_width);
		clamp(&middle_y, 0, env->settings.map_height);

		p->direction.d -= 1;
		start_x = middle_x + moves[p->direction.d][0] * i;
		start_y = middle_y + moves[p->direction.d][1] * i;
		p->direction.d += 2;
		end_x = middle_x + moves[p->direction.d][0] * i;
		end_y = middle_y + moves[p->direction.d][1] * i;
		p->direction.d -= 1;

		clamp(&start_x, 0, env->settings.map_width);
		clamp(&start_y, 0, env->settings.map_height);
		clamp(&end_x, 0, env->settings.map_width);
		clamp(&end_y, 0, env->settings.map_height);

		uint8_t	loot = 9;
		printf("%d %d %d %d\n", start_x, start_y, end_x, end_y);
		for (int16_t x = min(start_x, end_x); x <= max(start_x, end_x); x++)
			for (int16_t y = min(start_y, end_y); y <= max(start_y, end_y); y++)
			{
				clamp(&x, 0, env->settings.map_width);
				clamp(&y, 0, env->settings.map_height);
				printf("THERE\n");
				fflush(stdout);
				if (env->world.map[y][x].content.byte_size == 0)
					init_dynarray(&env->world.map[y][x].content, sizeof(uint8_t), 8);
				push_dynarray(&env->world.map[y][x].content, &loot, false);
			}
				
		//tx = p->tile_x + moves[p->direction][0];
		//ty = p->tile_y + moves[p->direction][1] * i;
		//p->tile_x - (int)floor(fov_width[i] / 2.0f);
	}
	return (ERR_NONE);
}

/*
uint8_t	cmd_inventory(t_env *env, t_player *p)
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

uint8_t	cmd_(t_env *env, t_player *p)
{
	return (ERR_NONE);
}
*/

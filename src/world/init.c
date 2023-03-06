#include "main.h"

static uint8_t	allocate_map_tiles(t_env *env)
{
	if (!(env->world.map = malloc(sizeof(t_tile*) * env->settings.map_height)))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; i < env->settings.map_height; i++)
	{
		if (!(env->world.map[i] = malloc(sizeof(t_tile) * env->settings.map_width)))
			return (ERR_MALLOC_FAILED);
		memset(env->world.map[i], 0, sizeof(t_tile) * env->settings.map_width);
	}

	return (ERR_NONE);
}

static uint8_t	fill_resources(t_env *env)
{
	uint8_t		code;
	const int	nb_tiles = env->settings.map_width * env->settings.map_height;

	if ((code = spawn_loot_pieces(env, (LOOT_DENSITY / 100.0f) * nb_tiles)) != ERR_NONE)
		return (code);

	return (ERR_NONE);
}

void	print_map(t_env *env)
{
	uint8_t	*t;
	for (uint32_t y = 0; y < env->settings.map_height; y++)
	{
		for (uint32_t x = 0; x < env->settings.map_width; x++)
		{
			t = dyacc(&env->world.map[y][x].content, 0);
			if (t == NULL)
				printf(". ");
			else
				printf("%d ", *t);
		}

		printf("\n");
	}
}

uint8_t	init_world(t_env *env)
{
	uint8_t	code;

	if ((code = allocate_map_tiles(env))
		|| (code = fill_resources(env)))
		return (code);

	return (ERR_NONE);
}

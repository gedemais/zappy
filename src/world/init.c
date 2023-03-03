#include "main.h"

static uint8_t	allocate_map_tiles(t_env *env)
{
	if (!(env->world.map = malloc(sizeof(t_tile*) * env->settings.map_height)))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; i < env->settings.map_height; i++)
	{
		if (!(env->world.map[i] = malloc(sizeof(t_tile) * env->settings.map_width)))
			return (ERR_MALLOC_FAILED);
	}

	return (ERR_NONE);
}

uint8_t	init_world(t_env *env)
{
	uint8_t	code;

	if ((code = allocate_map_tiles(env)))
		return (code);

	return (ERR_NONE);
}

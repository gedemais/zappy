#include "main.h"

uint8_t	spawn_loot_pieces(t_env *env, uint32_t count)
{
	uint8_t	loot;
	int		x, y;

	for (uint32_t i = 0; i < count; i++)
	{
		x = rand() % env->settings.map_width;
		y = rand() % env->settings.map_height;

		if (env->world.map[y][x].content.byte_size == 0
			&& init_dynarray(&env->world.map[y][x].content, sizeof(uint8_t), 4))
			return (ERR_MALLOC_FAILED);

		loot = rand() % 100 <= FOOD_DENSITY ? LOOT_FOOD : rand() % (LOOT_MAX - 1);

		if (push_dynarray(&env->world.map[y][x].content, &loot, false))
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

#include "main.h"

uint8_t	spawn_loot_pieces(t_env *env, uint32_t count)
{
	uint8_t	loot;
	int		x, y;

	// For each piece of loot needed to respect LOOT_DENSITY
	for (uint32_t i = 0; i < count; i++)
	{
		// Random position definition
		x = rand() % env->settings.map_width;
		y = rand() % env->settings.map_height;

		// Initialization of the content array in the tile located in [y][x]
		if (env->world.map[y][x].content.byte_size == 0
			&& init_dynarray(&env->world.map[y][x].content, sizeof(uint8_t), 4))
			return (ERR_MALLOC_FAILED);

		// Random loot item definition, in respect with LOOT_FOOD
		loot = rand() % 100 <= FOOD_DENSITY ? LOOT_FOOD : rand() % (LOOT_MAX - 1);

		// Add the newly created loot item to the content array of the tile.
		if (push_dynarray(&env->world.map[y][x].content, &loot, false))
			return (ERR_MALLOC_FAILED);
	}
	return (ERR_NONE);
}

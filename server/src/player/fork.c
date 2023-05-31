#include "main.h"

uint8_t	update_eggs(t_env *env)
{
	t_egg	*egg;
	for (int i = 0; i < env->world.eggs.nb_cells; i++)
	{
		egg = dyacc(&env->world.eggs, i);
		// Egg rots
		if (egg->hatch_time == 0)
		{
			printf("EGG ROTTED !\n");
			sleep(3);
			if (dynarray_extract(&env->world.eggs, i))
				return (ERR_MALLOC_FAILED);
		}
		else
			egg->hatch_time--;
	}
	return (ERR_NONE);
}

uint8_t	hatch_egg(t_env *env, t_player *p)
{
	uint8_t		loot;
	t_egg		new;
	t_dynarray	*tile_content;

	// Initialization of tile's content array (if not already allocated)
	tile_content = &env->world.map[p->tile_y][p->tile_x].content;
	if (tile_content->byte_size == 0
		&& dynarray_init(tile_content, sizeof(uint8_t), 4))
		return (ERR_MALLOC_FAILED);

	// Addition of the loot object on the map
	loot = HATCHING_EGG;
	tile_content = &env->world.map[p->tile_y][p->tile_x].content;
	if (dynarray_push(tile_content, &loot, false))
		return (ERR_MALLOC_FAILED);

	new.parent_connection = p->connection;
	new.hatch_time = 600;
	if (env->world.eggs.byte_size == 0 && dynarray_init(&env->world.eggs, sizeof(t_egg), env->world.teams.nb_cells))
		return (ERR_MALLOC_FAILED);

	// Addition of the egg to the hatching eggs list
	if (dynarray_push(&env->world.eggs, &new, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

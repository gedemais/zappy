#include "main.h"

uint8_t	update_eggs(t_env *env)
{
	t_egg		*egg;
	t_team		*team;

	for (int i = 0; i < env->world.eggs.nb_cells; i++)
	{
		egg = dyacc(&env->world.eggs, i);
		// Egg rots
		if (egg->hatch_time == 0)
		{
			printf("EGG ROTTED !\n");
			sleep(3);

			team = dyacc(&env->world.teams, egg->team);

			if (dynarray_extract(&env->world.eggs, i))
				return (ERR_MALLOC_FAILED);

			team->max_client--;
		}
		else
			egg->hatch_time--;
	}
	return (ERR_NONE);
}

uint8_t	check_connected_egg(t_env *env, uint16_t team)
{
	t_dynarray	*eggs;
	t_egg		*egg;
	t_egg		*oldest_egg;
	int			index = -1;

	eggs = &env->world.eggs;
	oldest_egg = dyacc(eggs, 0);
	for (int i = 0; i < eggs->nb_cells; i++)
	{
		egg = dyacc(eggs, i);
		if (egg->team == team && egg->hatch_time < oldest_egg->hatch_time)
		{
			oldest_egg = egg;
			index = i;
		}
	}

	if (index >= 0 && dynarray_extract(eggs, index))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

uint8_t	hatch_egg(t_env *env, t_player *p)
{
	uint8_t		loot;
	t_egg		new;
	t_dynarray	*tile_content;

	// Addition of the loot object on the map
	env->world.map[p->tile_y][p->tile_x].content[LOOT_HATCHING_EGG]++;

	new.team = p->team;
	new.hatch_time = 600;
	if (env->world.eggs.byte_size == 0 && dynarray_init(&env->world.eggs, sizeof(t_egg), env->world.teams.nb_cells))
		return (ERR_MALLOC_FAILED);

	// Addition of the egg to the hatching eggs list
	if (dynarray_push(&env->world.eggs, &new, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

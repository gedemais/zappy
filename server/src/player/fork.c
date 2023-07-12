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
			fprintf(stderr, "[EGG ROTTED] Egg %d is rotten\n", egg->id);
			env->gindex = egg->id;
			gevent_egg_rotted(env);

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
	int32_t		id;

	eggs = &env->world.eggs;
	oldest_egg = dyacc(eggs, 0);
	for (int i = 0; i < eggs->nb_cells; i++)
	{
		egg = dyacc(eggs, i);
		if (egg->team == team && egg->hatch_time < oldest_egg->hatch_time)
		{
			oldest_egg = egg;
			index = i;
			id = egg->id;
		}
	}

	env->gindex = id;
	gevent_player_connected_for_egg(env);

	if (index >= 0 && dynarray_extract(eggs, index))
		return (ERR_MALLOC_FAILED);

	gevent_egg_hatched(env);

	return (ERR_NONE);
}

uint8_t	hatch_egg(t_env *env, t_player *p)
{
	uint8_t		loot;
	t_egg		new;
	t_dynarray	*tile_content;

	env->gplayer = *p;
	gevent_player_lays_egg(env);
	// Addition of the loot object on the map
	env->world.map[p->tile_y][p->tile_x].content[LOOT_HATCHING_EGG]++;

	new.team = p->team;
	new.hatch_time = 600;
	new.id = rand() * rand() * rand();
	new.id *= (new.id < 0) ? -1 : 1;
	if (env->world.eggs.byte_size == 0 && dynarray_init(&env->world.eggs, sizeof(t_egg), env->world.teams.nb_cells))
		return (ERR_MALLOC_FAILED);

	// Addition of the egg to the hatching eggs list
	if (dynarray_push(&env->world.eggs, &new, false))
		return (ERR_MALLOC_FAILED);

	env->gindex = new.id;
	gevent_player_layed_egg(env);

	return (ERR_NONE);
}

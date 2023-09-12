#include "main.h"

uint8_t	update_eggs(t_env *env)
{
	t_egg		*egg;
	t_team		*team;

	log_function((char*)__FUNCTION__);
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

uint8_t	check_connected_egg(t_env *env, t_player *p)
{
	t_dynarray	*eggs;
	t_egg		*egg;
	t_egg		*oldest_egg;
	int			index = 0;
	bool		found = false;

	printf("NEW_PLAYER\n");
	eggs = &env->world.eggs;
	oldest_egg = dyacc(eggs, 0);
	for (int i = 0; i < eggs->nb_cells; i++)
	{
		egg = dyacc(eggs, i);
		if (egg->team == p->team)
		{
			if (!found)
			{
				oldest_egg = egg;
				index = i;
				found = true;
			}
			else if (egg->hatch_time < oldest_egg->hatch_time)
			{
				oldest_egg = egg;
				index = i;
			}
		}
	}

	if (!found)
	{
		env->gplayer = *p;
		gevent_player_new(env);
		printf("NEW_PLAYER : EGG NOT FOUND\n");
		fflush(stdout);
		//sleep(3);
		return (ERR_NONE);
	}

	printf("NEW_PLAYER : EGG FOUND\n");
	fflush(stdout);
	//sleep(3);

	p->tile_x = oldest_egg->x;
	p->tile_y = oldest_egg->y;

	env->gplayer = *p;
	gevent_player_new(env);

	env->gindex = oldest_egg->id;
	gevent_player_connected_for_egg(env);


	//printf("connection : %d %d %d\n", env->gindex, p->tile_x, p->tile_y);
	//fflush(stdout);
	//sleep(5);
	dynarray_extract(eggs, index);
	return (gevent_egg_hatched(env));
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
	new.id = rand() * rand() * rand();
	new.id *= (new.id < 0) ? -1 : 1;
	new.pid = p->pid;
	new.x = p->tile_x;
	new.y = p->tile_y;
	//printf("hatching : %d %d %d\n", new.id, p->tile_x, p->tile_y);
	//fflush(stdout);
	//sleep(5);
	if (env->world.eggs.byte_size == 0 && dynarray_init(&env->world.eggs, sizeof(t_egg), env->world.teams.nb_cells))
		return (ERR_MALLOC_FAILED);

	// Addition of the egg to the hatching eggs list
	if (dynarray_push(&env->world.eggs, &new, false))
		return (ERR_MALLOC_FAILED);

	env->gindex = new.id;
	env->gplayer = *p;
	gevent_player_layed_egg(env);

	return (ERR_NONE);
}

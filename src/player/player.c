#include "main.h"

void	teams_log(t_env *env)
{
	t_team		*t;
	t_player	*p;

	printf("============= TEAMS LOG =============\n");
	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		printf("***** Team %s *****\n", t->name);
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);
			printf("Player %d | orientation : %d | x : %d | y : %d | food : %d | satiety : %d\n", player, *(uint8_t*)&p->direction, p->tile_x, p->tile_y, p->inventory[LOOT_FOOD], p->satiety);
		}
	}
	printf("=====================================\n");
}

void	update_food(t_player *p)
{
	if (p->satiety <= 0 && p->inventory[LOOT_FOOD] == 0)
		p->alive = false;
	else if (p->satiety == 0)
	{
		p->inventory[LOOT_FOOD]--;
		p->satiety += 126;
	}
	p->satiety--;
}

uint8_t	add_player(t_env *env, t_team *team)
{
	t_player	new;
	uint8_t		d;

	d = rand() % DIR_MAX;
	if (team->players.byte_size == 0
		&& init_dynarray(&team->players, sizeof(t_player), 6))
		return (ERR_MALLOC_FAILED);

	memset(&new, 0, sizeof(t_player));
	new.inventory[LOOT_FOOD] = 10;
	new.tile_x = rand() % env->settings.map_width;
	new.tile_y = rand() % env->settings.map_height;
	new.level = 8;
	new.alive = true;
	new.direction = *((t_direction*)&d);

	if (push_dynarray(&team->players, &new, false))
		return (ERR_MALLOC_FAILED);

	if (env->world.map[new.tile_y][new.tile_x].content.byte_size == 0
		&& init_dynarray(&env->world.map[new.tile_y][new.tile_x].content, sizeof(uint8_t), 4))
		return (ERR_MALLOC_FAILED);

	uint8_t	loot = 255;
	push_dynarray(&env->world.map[new.tile_y][new.tile_x].content, &loot, false);

	return (ERR_NONE);
}



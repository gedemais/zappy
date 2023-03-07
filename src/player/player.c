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
			printf("Player %d | x : %d | y : %d | food : %d | satiety : %d\n", player, p->tile_x, p->tile_y, p->food, p->satiety);
		}
	}
	printf("=====================================\n");
}

void	update_food(t_player *p)
{
	if (p->satiety <= 0 && p->food == 0)
		p->alive = false;
	else if (p->satiety == 0)
	{
		p->food--;
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
	new.tile_x = rand() % env->settings.map_width;
	new.tile_y = rand() % env->settings.map_height;
	new.food = 10;
	new.level = 8;
	new.alive = true;
	new.direction = *((t_direction*)&d);

	if (push_dynarray(&team->players, &new, false))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}



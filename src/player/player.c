#include "main.h"

void	teams_log(t_env *env)
{
	t_team		*t;
	t_player	*p;

	printf("============= TEAMS LOG =============\n");
	printf("%d pending players\n", env->world.pending.players.nb_cells);
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

static uint8_t	update_food(t_env *env, t_player *p)
{
	// If player's satiety is zero and have no food, he will die.
	if (p->satiety <= 0 && p->inventory[LOOT_FOOD] == 0)
		return (remove_player(env, p->connection));
	else if (p->satiety == 0)
	{ // Eating mechanism
		p->inventory[LOOT_FOOD]--;
		p->satiety += 126;
	}
	// Normal satiety decrementation for each tick
	p->satiety--;
	return (ERR_NONE);
}

// Update players's stats at each tick
uint8_t			update_players(t_env *env)
{
	uint8_t		code;
	t_team		*t;
	t_player	*p;

	// Iterate on every team playing on the server
	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		// Iterate on every player if t
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);

			if (p->alive == true && (code = update_food(env, p)) != ERR_NONE)
				return (code);
		}
	}

	//teams_log(env);
	return (ERR_NONE);
}

static void		fill_player(t_env *env, t_player *new, int connection)
{
	int	d = rand() % DIR_MAX;
	// Wipe new player variable
	memset(new, 0, sizeof(t_player));

	new->inventory[LOOT_FOOD] = 10; // Food starting quantity

	// Player's random coordinates definition
	new->tile_x = rand() % env->settings.map_width;
	new->tile_y = rand() % env->settings.map_height;

	new->level = 8; // Starting level
	new->alive = true; // It's ALIVE !!!
	new->direction = *((t_direction*)&d); // Direction assignment
	new->connection = connection; // Connection fd assignment
}

// Removes player connected to the server by connection fd
uint8_t	remove_player(t_env *env, int connection_fd)
{
	t_team		*team;
	t_player	*player;

	if (push_dynarray(&env->world.teams, &env->world.pending, false))
		return (ERR_MALLOC_FAILED);
	for (int t = 0; t < env->world.teams.nb_cells; t++)
	{
		team = dyacc(&env->world.teams, t);
		for (int p = 0; p < team->players.nb_cells; p++)
		{
			player = dyacc(&team->players, p);
			if (player->connection == connection_fd)
			{
				if (extract_dynarray(&team->players, p)
					|| pop_dynarray(&env->world.teams, false))
					return (ERR_MALLOC_FAILED);
				team->connected--;
				return (ERR_NONE);
			}
		}
	}

	if (pop_dynarray(&env->world.teams, false))
		return (ERR_MALLOC_FAILED);

	assert(false);
	return (ERR_NONE);
}

// Add a new payer to a specific team
uint8_t			add_player(t_env *env, t_team *team, int connection)
{
	t_player	new;
	uint8_t		d = rand() % DIR_MAX; // Player's random spawn direction definition

	// If this team does not count any player
	if (team->players.byte_size == 0
		&& init_dynarray(&team->players, sizeof(t_player), 6)) // Init the array of players
		return (ERR_MALLOC_FAILED);

	fill_player(env, &new, connection);
	// Add the newly created player to team
	if (push_dynarray(&team->players, &new, false))
		return (ERR_MALLOC_FAILED);

	// Init the dynamic array containing loot on the tile where the player is located
	if (env->world.map[new.tile_y][new.tile_x].content.byte_size == 0
		&& init_dynarray(&env->world.map[new.tile_y][new.tile_x].content, sizeof(uint8_t), 4))
		return (ERR_MALLOC_FAILED);

	uint8_t	loot = 255;
	// Add a 'player' loot item to the tile content array (for now 255 in uint8_t)
	if (push_dynarray(&env->world.map[new.tile_y][new.tile_x].content, &loot, false))
		return (ERR_MALLOC_FAILED);

	team->connected++;

	return (ERR_NONE);
}



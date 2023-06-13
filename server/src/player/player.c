#include "main.h"

void	teams_log(t_env *env)
{
	t_team		*t;
	t_player	*p;
	t_egg		*egg;

	printf("============= TEAMS LOG =============\n");
	printf("%d pending players\n", env->world.pending.players.nb_cells);
	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		printf("***** Team %s (%d / %d) *****\n", t->name, t->connected, t->max_client);
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);
			printf("Player %d | orientation : %d | x : %d | y : %d | food : %d | satiety : %d | commands : %d | connection : %d\n", player, *(uint8_t*)&p->direction, p->tile_x, p->tile_y, p->inventory[LOOT_FOOD], p->satiety, p->cmd_queue.nb_cells, *p->connection);
		}
	}
	printf("=====================================\n");
}

uint8_t	kill_player(t_env *env, t_player *p, bool disconnected)
{
	printf("THERE3\n");
	fflush(stdout);
	if (disconnected == false)
	{
		FLUSH_RESPONSE
		strcat(env->buffers.response, "mort\n");
		response(env, p);
	}
	else
		close(*p->connection);

	*p->connection = -1;

	return (remove_player(env, *p->connection));
}

static uint8_t	update_food(t_env *env, t_player *p)
{
	// If player's satiety is zero and have no food, he will die.
	if (p->satiety <= 0 && p->inventory[LOOT_FOOD] == 0)
		return (kill_player(env, p, false));
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

static void		fill_player(t_env *env, t_player *new, int *connection)
{
	int	d = rand() % DIR_MAX;
	// Wipe new player variable
	memset(new, 0, sizeof(t_player));

	new->inventory[LOOT_FOOD] = 10; // Food starting quantity

	// PID generation
	new->pid = rand() * rand() * rand();
	// Player's random coordinates definition
	//new->tile_x = rand() % env->settings.map_width;
//	new->tile_y = rand() % env->settings.map_height;
	new->tile_x = 10;
	new->tile_y = 10;
	(void)env;

	new->level = 1; // Starting level
	new->alive = true; // It's ALIVE !!!
	new->direction.d = (uint8_t)d; // Direction assignment
	new->connection = connection; // Connection fd assignment
}

// Removes player connected to the server by connection fd
uint8_t	remove_player(t_env *env, int connection_fd)
{
	t_team		*team;
	t_player	*player;
	uint8_t		code;

	if (dynarray_push(&env->world.teams, &env->world.pending, false))
		return (ERR_MALLOC_FAILED);

	for (int t = 0; t < env->world.teams.nb_cells; t++)
	{
		team = dyacc(&env->world.teams, t);
		for (int p = 0; p < team->players.nb_cells; p++)
		{
			player = dyacc(&team->players, p);
			if (*player->connection == connection_fd)
			{
				dynarray_free(&player->cmd_queue);
				if ((code = remove_player_from_tile(env, player->tile_x, player->tile_y)) != ERR_NONE)
					return (code);

				if (dynarray_extract(&team->players, p))
					return (ERR_MALLOC_FAILED);

				dynarray_pop(&env->world.teams, false);

				team->connected--;
				team->max_client -= (team->max_client > 1 ? 1 : 0);

				return (ERR_NONE);
			}
		}
	}

	dynarray_pop(&env->world.teams, false);

	assert(false);
	return (ERR_NONE);
}

// Add a new payer to a specific team
uint8_t			add_player(t_env *env, t_team *team, int *connection)
{
	t_player	new;
	uint8_t		d = rand() % DIR_MAX; // Player's random spawn direction definition
	uint8_t		code;

	// If this team does not count any player
	if (team->players.byte_size == 0
		&& dynarray_init(&team->players, sizeof(t_player), 6)) // Init the array of players
		return (ERR_MALLOC_FAILED);

	// Generate player metadata
	fill_player(env, &new, connection);

	// Add the newly created player to team
	if (dynarray_push(&team->players, &new, false))
		return (ERR_MALLOC_FAILED);

	// Add the newly created player as a loot object in its spawn tile
	env->world.map[new.tile_y][new.tile_x].content[LOOT_PLAYER]++;
	team->connected++;

	if ((code = check_connected_egg(env, new.team)))
		return (code);

	return (ERR_NONE);
}

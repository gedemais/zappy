#include "main.h"

void	log_function(char *s)
{
	PUTTIME()
	fprintf(stderr, "Function %s launched\n", s);
}

void	teams_log(t_env *env, bool log)
{
	t_team		*t;
	t_player	*p;
	t_egg		*egg;

	fprintf(log ? stderr : stdout, "======== TEAMS LOG ========\n");
	fprintf(log ? stderr : stdout, "%d pending players\n", env->world.pending.players.nb_cells);
	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		fprintf(log ? stderr : stdout, "***** Team %s (%d / %d) *****\n", t->name, t->connected, t->max_client);
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);
			fprintf(log ? stderr : stdout, "Player %d | level %d | food : %d | satiety : %d | commands : %d\n", player, p->level, p->inventory[LOOT_FOOD], p->satiety, p->cmd_queue.nb_cells);
		}
		for (int e = 0; e < env->world.eggs.nb_cells; e++)
		{
			egg = dyacc(&env->world.eggs, e);
			fprintf(log ? stderr : stdout, "Egg %d | x : %d | y : %d | Hatch time : %d | Team : %d\n", egg->id, egg->x, egg->y, egg->hatch_time, egg->team);
		}
	}
	fprintf(log ? stderr : stdout, "===========================\n");
}

uint8_t	kill_player(t_env *env, t_player *p, bool disconnected)
{
	struct timeval	killed;
	uint8_t			code;

	if (p == NULL)
		return (ERR_NONE);
	PUTTIME()
	fprintf(stderr, "[PLAYER DEATH] message : Client %d died !\n", *p->connection);
	if (disconnected == false)
	{
		FLUSH_RESPONSE
		strcat(env->buffers.response, "mort\n");
		response(env, p);
	}
	else
		close(*p->connection);

	code = remove_player(env, *p->connection);

	return (code);
}

static uint8_t	update_level(t_env *env, t_player *p)
{
	char	r[128];
	char	*lvl;
	uint8_t	code;

	if (p->elevation > 0)
		p->elevation--;
	else if (p->elevation == 0)
	{
		p->level++;
		p->elevation = -1;

		bzero(r, sizeof(char) * 128);

		strcat(r, "niveau actuel : ");

		if (!(lvl = ft_itoa((int)p->level)))
			return (ERR_MALLOC_FAILED);

		strcat(r, lvl);
		free(lvl);
		strcat(r, "\n");

		if ((code = send_response(env, p, r))
			|| (code = gcmd_player_level(env)))
			return (code);

		return (gresponse(env));
	}
	return (ERR_NONE);
}

uint8_t	update_inventories(t_env *env)
{
	t_team		*team;
	t_player	*p;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		team = dyacc(&env->world.teams, i);
		for (int j = 0; j < team->players.nb_cells; j++)
		{
			p = dyacc(&team->players, j);
			env->gplayer = *p;
			gcmd_player_inventory(env);
		}
	}
	return (gresponse(env));
}

static uint8_t	update_food(t_env *env, t_player *p)
{
	uint8_t	code;

	// If player's satiety is zero and have no food, he will die.
	if (p->satiety <= 0 && p->inventory[LOOT_FOOD] == 0)
	{
		env->gplayer = *p;
		if ((code = gevent_player_died(env)))
			return (code);
		return (kill_player(env, p, false));
	}
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

	log_function((char*)__FUNCTION__);
	// Iterate on every team playing on the server
	for (int team = 0; team < env->world.teams.nb_cells; team++)
	{
		t = dyacc(&env->world.teams, team);
		// Iterate on every player if t
		for (int player = 0; player < t->players.nb_cells; player++)
		{
			p = dyacc(&t->players, player);

			if ((code = update_level(env, p)))
				return (code);

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

	//d = DIR_NORTH;
	// Wipe new player variable
	memset(new, 0, sizeof(t_player));

	new->inventory[LOOT_FOOD] = 10; // Food starting quantity

	// PID generation
	new->pid = rand() * rand() * rand();
	new->pid *= (new->pid < 0) ? -1 : 1;

	// Player's random coordinates definition
	new->tile_x = rand() % env->settings.map_width;
	new->tile_y = rand() % env->settings.map_height;

	new->level = 1; // Starting level
	new->alive = true; // It's ALIVE !!!
	new->direction.d = (uint8_t)d; // Direction assignment
	new->connection = connection; // Connection fd assignment
	new->elevation = -1;
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
				*player->connection = -1;
				bool pending = (t == env->world.teams.nb_cells - 1);
				PUTTIME()
				fprintf(stderr, "[CLIENT REMOVED] Client connected to slot %d was removed from team {%s}.\n", connection_fd, pending ? "pending" : team->name);

				dynarray_free(&player->cmd_queue);
				if ((code = remove_player_from_tile(env, player->tile_x, player->tile_y)) != ERR_NONE)
					return (code);

				if (dynarray_extract(pending ? &env->world.pending.players : &team->players, p))
					return (ERR_MALLOC_FAILED);

				fprintf(stderr, "%d players left in team %s\n", team->players.nb_cells, team->name);

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
uint8_t			add_player(t_env *env, t_team *team, int *connection, t_player *pending)
{
	t_player	new;
	uint8_t		d = rand() % DIR_MAX; // Player's random spawn direction definition
	uint8_t		code;

	// If this team does not count any player
	if (team->players.byte_size == 0
		&& dynarray_init(&team->players, sizeof(t_player), 6)) // Init the array of players
		return (ERR_MALLOC_FAILED);

	if (team == &env->world.pending)
		// Generate player metadata
		fill_player(env, &new, connection);
	else if (pending != NULL)
		new = *pending;

	// Add the newly created player to team
	if (dynarray_push(&team->players, &new, false))
		return (ERR_MALLOC_FAILED);

	// Add the newly created player as a loot object in its spawn tile
	env->world.map[new.tile_y][new.tile_x].content[LOOT_PLAYER]++;
	team->connected++;

	return (ERR_NONE);
}

#include "main.h"

static uint8_t	take_req(t_env *env, char **tokens, t_player *p, bool *ret)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	if (tokens[1] == NULL)
		return (ERR_NONE);

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(tokens[1], loot_titles[i]) == 0)
			loot = i;

	if (loot != 255 && tile->content[loot] > 0)
		*ret = true;

	return (ERR_NONE);
}

static uint8_t	put_req(t_env *env, char **tokens, t_player *p, bool *ret)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	if (tokens[1] == NULL)
		return (ERR_NONE);

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(tokens[1], loot_titles[i]) == 0)
			loot = i;

	if (loot == 255 || p->inventory[loot] == 0)
		return (ERR_NONE);

	*ret = true;

	return (ERR_NONE);
}

static uint8_t	kick_req(t_env *env, char **tokens, t_player *p, bool *ret)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];

	(void)tokens;
	if (tile->content[LOOT_PLAYER] > 1)
		*ret = true;

	return (ERR_NONE);
}

static uint8_t	fork_req(t_env *env, char **tokens, t_player *p, bool *ret)
{
	t_team	*team;

	(void)tokens;

	team = dyacc(&env->world.teams, p->team);

	if (team->max_client - team->connected > 0)
		*ret = true;

	return (ERR_NONE);
}

// ==================== INCANTATION REQUIREMENTS  ======================

static bool	check_resources(t_env *env, t_player *p)
{
	t_tile	*tile;

	tile = &env->world.map[p->tile_y][p->tile_x];
	for (unsigned int i = 0; i < LOOT_MAX; i++)
	{
		if (tile->content[i] < lvl_up_requirements[p->level - 1][i])
			return (false);
	}
	return (true);
}

static uint8_t	check_incantation_group(t_env *env, t_player *p, bool *ret)
{
	t_team			*t;
	t_player		*pl;
	unsigned int	nb_players = 0;
	bool			coords;
	t_dynarray		group;

	if (dynarray_init(&group, sizeof(t_player*), 6))
		return (ERR_MALLOC_FAILED);

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		for (int j = 0; j < env->world.teams.nb_cells; j++)
		{
			pl = dyacc(&t->players, j);
			if (dynarray_push(&group, pl, false))
			{
				dynarray_free(&group);
				return (ERR_MALLOC_FAILED);
			}
			coords = (bool)(pl->tile_x == p->tile_x && pl->tile_y == p->tile_y);
			if (coords && pl->level == p->level)
				nb_players++;
		}
	}

	if (nb_players >= lvl_up_requirements[p->level - 1][LOOT_MAX])
	{
		t_player	**player;
		for (int i = 0; i < group.nb_cells; i++)
		{
			player = dyacc(&group, i);
			((t_player*)*player)->elevation = 300;
		}
		*ret = true;
		return (ERR_NONE);
	}
	return (ERR_NONE);
}

static void	consume_resources(t_env *env, t_player *p)
{
	t_tile	*tile;

	tile = &env->world.map[p->tile_y][p->tile_x];
	for (unsigned int i = 0; i < LOOT_MAX; i++)
	{
		for (unsigned int n = 0; n < lvl_up_requirements[p->level - 1][i]; n++)
			tile->content[i]--;
	}
}

static uint8_t	incantation_req(t_env *env, char **tokens, t_player *p, bool *ret)
{
	uint8_t	code;

	(void)tokens;
	if (!check_resources(env, p))
		return (ERR_NONE);

	if ((code = check_incantation_group(env, p, ret)))
		return (code);

	if (*ret == false)
		return (ERR_NONE);

	consume_resources(env, p);

	env->gplayer = p;
	if ((code = gevent_incantation_launch(env)))
		return (code);

	return (ERR_NONE);
}

// =====================================================================

uint8_t	check_requirements(t_env *env, char **tokens, t_player *player, int cmd, bool *ret)
{
	uint8_t		code;
	uint8_t		(*requirements_checks[CMD_MAX])(t_env *, char **, t_player *, bool *) = {
		[CMD_ADVANCE] = NULL,
		[CMD_RIGHT] = NULL,
		[CMD_LEFT] = NULL,
		[CMD_SEE] = NULL,
		[CMD_INVENTORY] = NULL,
		[CMD_TAKE] = take_req,
		[CMD_PUTDOWN] = put_req,
		[CMD_KICK] = kick_req,
		[CMD_BROADCAST] = NULL,
		[CMD_INCANTATION] = incantation_req,
		[CMD_FORK] = fork_req,
		[CMD_CONNECT_NBR] = NULL
	};

	if (requirements_checks[cmd] != NULL)
	{
		if ((code = requirements_checks[cmd](env, tokens, player, ret)))
			return (code);
	}
	else
		*ret = true;

	return (ERR_NONE);
}

#include "main.h"

uint8_t	cmd_see(t_env *env, t_player *p, bool send_response)
{
	t_view_ranges	ranges;
	int16_t			tx, ty;

	(void)send_response;
	if (dynarray_push(&env->buffers.view, &env->world.map[p->tile_y][p->tile_x], false))
	{
		dynarray_clear(&env->buffers.view);
		return (ERR_MALLOC_FAILED);
	}
	for (uint8_t i = 0; i < p->level; i++)
	{
		compute_view_ranges(env, &ranges, p, i);
		for (int16_t x = min(ranges.start_x, ranges.end_x); x <= max(ranges.start_x, ranges.end_x); x++)
			for (int16_t y = min(ranges.start_y, ranges.end_y); y <= max(ranges.start_y, ranges.end_y); y++)
			{
				tx = x;
				ty = y;
				clamp(&tx, 0, env->settings.map_width);
				clamp(&ty, 0, env->settings.map_height);
				if (dynarray_push(&env->buffers.view, &env->world.map[ty][tx], false))
				{
					dynarray_clear(&env->buffers.view);
					return (ERR_MALLOC_FAILED);
				}
			}
	}
	send_see_response(env, &env->buffers.view, p);
	dynarray_clear(&env->buffers.view);
	return (ERR_NONE);
}

uint8_t	cmd_inventory(t_env *env, t_player *p, bool send_response)
{
	char	*str;

	(void)send_response;
	FLUSH_RESPONSE
	strcat(env->buffers.response, "{");
	for (uint16_t i = 0; i < LOOT_MAX; i++)
	{
		strcat(env->buffers.response, loot_titles[i]);
		strcat(env->buffers.response, " ");

		if (!(str = ft_itoa((int)p->inventory[i])))
			return (ERR_MALLOC_FAILED);

		strcat(env->buffers.response, str);
		free(str);

		strcat(env->buffers.response, ", ");
	}

	strcat(env->buffers.response, "ttl ");

	if (!(str = ft_itoa(p->inventory[LOOT_FOOD] * 126 + p->satiety)))
		return (ERR_MALLOC_FAILED);

	strcat(env->buffers.response, str);
	free(str);

	strcat(env->buffers.response, "}\n");
	response(env, p);
	return (ERR_NONE);
}

uint8_t	cmd_take(t_env *env, t_player *p, bool send_response)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	if (env->buffers.cmd_params[0] == NULL)
		return (send_response ? send_ko(env, p) : ERR_NONE);

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_params[0], loot_titles[i]) == 0)
			loot = i;

	if (loot == 255)
		return (send_response ? send_ko(env, p) : ERR_NONE);

	if (tile->content[loot] > 0)
	{
		tile->content[loot]--;
		p->inventory[(int)loot]++;
	}

	return (send_response ? send_ok(env, p) : ERR_NONE);
}

uint8_t	cmd_put(t_env *env, t_player *p, bool send_response)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	if (env->buffers.cmd_params[0] == NULL)
		return (send_response ? send_ko(env, p) : ERR_NONE);

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_params[0], loot_titles[i]) == 0)
			loot = i;

	if (loot == 255 || p->inventory[loot] == 0)
		return (send_response ? send_ko(env, p) : ERR_NONE);

	p->inventory[loot]--;
	tile->content[loot]++;
	return (send_response ? send_ko(env, p) : ERR_NONE);
}

uint8_t	cmd_kick(t_env *env, t_player *p, bool send_response)
{
	t_direction	dir;
	char		*s;
	bool		kicked = false;
	int			direction;

	for (int t = 0; t < env->world.teams.nb_cells; t++)
	{
		t_team		*te;
		t_player	*pl;

		for (int team = 0; team < env->world.teams.nb_cells; team++)
		{
			te = dyacc(&env->world.teams, team);
			for (int player = 0; player < te->players.nb_cells; player++)
			{
				pl = dyacc(&te->players, player);
				if (pl->tile_x == p->tile_x && pl->tile_y == p->tile_y && p != pl)
				{
					kicked = true;

					dir = pl->direction;
					pl->direction = p->direction;
					cmd_advance(env, pl, false);
					pl->direction = dir;

					dir.d -= p->direction.d; // Get kick direction in the kicked player referential
					direction = dir.d; // Int conversion

					FLUSH_RESPONSE
					strcat(env->buffers.response, "deplacement ");

					if (!(s = ft_itoa(direction))) // Invalid for now
						return (ERR_MALLOC_FAILED);

					strcat(env->buffers.response, s);
					free(s);
					strcat(env->buffers.response, "\n");

					clamp(&pl->tile_x, 0, env->settings.map_width);
					clamp(&pl->tile_y, 0, env->settings.map_height);
					response(env, pl);
				}
			}
		}
	}
	if (send_response)
	{
		FLUSH_RESPONSE
		strcat(env->buffers.response, kicked ? "ok\n" : "ko\n");
		response(env, p);
	}

	return (ERR_NONE);
}


uint8_t	cmd_broadcast(t_env *env, t_player *p, bool send_response)
{
	uint8_t	code;
	(void)send_response;

	FLUSH_RESPONSE
	if (env->buffers.cmd_params[0] == NULL)
	{
		strcat(env->buffers.response, "ko\n");
		response(env, p);
		return (ERR_NONE);
	}

	if ((code = deliver_messages(env, p)) != ERR_NONE)
		return (code);

	FLUSH_RESPONSE
	strcat(env->buffers.response, "ok\n");
	response(env, p);
	return (ERR_NONE);
}

uint8_t	cmd_connect_nbr(t_env *env, t_player *p, bool send_response)
{
	t_team	*team;
	char	*remaining, *used;

	team = dyacc(&env->world.teams, p->team);

	if (send_response)
	{
		FLUSH_RESPONSE
		if (!(remaining = ft_itoa(team->max_client - team->connected)))
			return (ERR_MALLOC_FAILED);
		strcat(env->buffers.response, remaining);
		strcat(env->buffers.response, ", ");
		free(remaining);

		if (!(used = ft_itoa(team->connected)))
			return (ERR_MALLOC_FAILED);
		strcat(env->buffers.response, used);
		free(used);

		strcat(env->buffers.response, "\n");

		response(env, p);
	}
	return (ERR_NONE);
}

/*
uint8_t	cmd_incantation(t_env *env, t_player *p, bool send_response)
{
	return (ERR_NONE);
}
*/

uint8_t	cmd_fork(t_env *env, t_player *p, bool send_response)
{
	t_team		*team;
	uint8_t		code;

	FLUSH_RESPONSE
	team = dyacc(&env->world.teams, p->team);
	if (team->max_client >= 6)
	{
		if (send_response)
		{
			strcat(env->buffers.response, "ko\n");
			response(env, p);
		}
		return (ERR_NONE);
	}

	if ((code = hatch_egg(env, p)))
		return (code);

	team->max_client++;

	if (send_response)
	{
		strcat(env->buffers.response, "ok\n");
		response(env, p);
	}

	return (ERR_NONE);
}


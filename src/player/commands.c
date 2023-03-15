#include "main.h"

uint8_t	cmd_see(t_env *env, t_player *p, bool send_response)
{
	t_view_ranges	ranges;
	int16_t			tx, ty;

	(void)send_response;
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
				if (push_dynarray(&env->buffers.view, &env->world.map[ty][tx], false))
				{
					clear_dynarray(&env->buffers.view);
					return (ERR_MALLOC_FAILED);
				}
			}
	}
	send_see_response(env, &env->buffers.view, p);
	clear_dynarray(&env->buffers.view);
	return (ERR_NONE);
}

uint8_t	cmd_inventory(t_env *env, t_player *p, bool send_response)
{
	char	*amnt;

	(void)send_response;
	FLUSH_RESPONSE
	strcat(env->buffers.response, "{");
	for (uint16_t i = 0; i < LOOT_MAX; i++)
	{
		if (!(amnt = ft_itoa((int)p->inventory[i])))
			return (ERR_MALLOC_FAILED);

		strcat(env->buffers.response, amnt);
		free(amnt);

		strcat(env->buffers.response, " ");
		strcat(env->buffers.response, loot_titles[i]);

		if (i < LOOT_MAX - 1)
			strcat(env->buffers.response, ", ");
	}
	strcat(env->buffers.response, "}");
	response(env, p);
	return (ERR_NONE);
}

uint8_t	cmd_take(t_env *env, t_player *p, bool send_response)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	*loot_ptr;
	uint8_t	loot = 255;
	bool	found = false;

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_params[0], loot_titles[i]) == 0)
			loot = i;

	for (int i = 0; i < tile->content.nb_cells; i++)
	{
		loot_ptr = dyacc(&tile->content, i);
		if ((uint8_t)*loot_ptr == loot && (found = true)
			&& extract_dynarray(&tile->content, i))
			return (ERR_MALLOC_FAILED);
	}

	FLUSH_RESPONSE
	if (loot == 255 || tile->content.nb_cells == 0 || found == false)
		strcat(env->buffers.response, "ko");
	else
	{
		p->inventory[(int)loot]++;
		strcat(env->buffers.response, "ok");
	}
	if (send_response)
		response(env, p);
	return (ERR_NONE);
}

uint8_t	cmd_put(t_env *env, t_player *p, bool send_response)
{
	t_tile	*tile = &env->world.map[p->tile_y][p->tile_x];
	uint8_t	loot = 255;

	for (uint8_t i = 0; i < LOOT_MAX; i++)
		if (strcmp(env->buffers.cmd_params[0], loot_titles[i]) == 0)
			loot = i;

	FLUSH_RESPONSE
	if (loot == 255 || p->inventory[loot] == 0)
		strcat(env->buffers.response, "ko");
	else
	{
		strcat(env->buffers.response, "ok");
		p->inventory[loot]--;
		if ((tile->content.byte_size == 0 && init_dynarray(&tile->content, sizeof(uint8_t), 4))
			|| push_dynarray(&tile->content, &loot, false))
			return (ERR_MALLOC_FAILED);
	}

	if (send_response)
		response(env, p);

	return (ERR_NONE);
}

uint8_t	cmd_kick(t_env *env, t_player *p, bool send_response)
{
	t_direction	dir;
	char		*s;
	bool		kicked = false;

	for (int t = 0; t < env->world.teams.nb_cells; t++)
	{
		t_team		*te;
		t_player	*pl;

		if (!(s = ft_itoa(directions[(int)p->direction.d])))
			return (ERR_MALLOC_FAILED);

		FLUSH_RESPONSE
		strcat(env->buffers.response, "deplacement ");
		strcat(env->buffers.response, s);

		free(s);

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
		strcat(env->buffers.response, kicked ? "ok" : "ko");
		response(env, p);
	}

	return (ERR_NONE);
}


uint8_t	cmd_broadcast(t_env *env, t_player *p, bool send_response)
{
	uint8_t	code;
	(void)send_response;

	if ((code = build_message_from_params(env)) != ERR_NONE
		|| (code = deliver_message(env, p)) != ERR_NONE)
		return (code);

	FLUSH_RESPONSE
	strcat(env->buffers.response, "ok");
	response(env, p);
	return (ERR_NONE);
}

/*
uint8_t	cmd_incantation(t_env *env, t_player *p, bool send_response)
{
	return (ERR_NONE);
}

uint8_t	cmd_fork(t_env *env, t_player *p, bool send_response)
{
	return (ERR_NONE);
}

uint8_t	cmd_connect_nbr(t_env *env, t_player *p, bool send_response)
{
	return (ERR_NONE);
}
*/

#include "main.h"

uint8_t	auth_send_welcome(t_env *env, t_player *p)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, "BIENVENUE\n");
	response(env, p);
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	build_granting_response(t_env *env, char *response, t_team *team, t_player *p)
{
	char	*nb_client, *x, *y;

	(void)p;
	if (!(nb_client = ft_itoa(team->max_client - team->connected))
		|| !(x = ft_itoa((int)env->settings.map_width))
		|| !(y = ft_itoa((int)env->settings.map_height)))
		return (ERR_MALLOC_FAILED);

	FLUSH_RESPONSE
	strcat(response, nb_client);
	strcat(response, "\n");
	strcat(response, x);
	strcat(response, " ");
	strcat(response, y);
	strcat(response, "\n");

	free(nb_client);
	free(x);
	free(y);

	env->gplayer = *p;
	return (gevent_player_new(env));
}

static uint8_t	remove_pending_player(t_env *env, t_player *p)
{
	t_player	*player;
	uint8_t		*loot;

	for (int i = 0; i < env->world.pending.players.nb_cells; i++)
	{
		player = dyacc(&env->world.pending.players, i);
		if (player->connection == p->connection)
		{
			if (remove_player_from_tile(env, p->tile_x, p->tile_y)
				|| dynarray_extract(&env->world.pending.players, i))
				return (ERR_MALLOC_FAILED);
			return (ERR_NONE);
		}
	}
	assert(false);
	return (ERR_NONE);
}

static uint8_t	auth_granting(t_env *env, t_player *p)
{
	t_team	*team;
	uint8_t	code;

	team = dyacc(&env->world.teams, (int)p->team);

	if ((code = build_granting_response(env, env->buffers.response, team, p)))
		return (code);

	response(env, p);
	p->auth_step++;

	if ((code = add_player(env, team, p->connection, p)))
		return (ERR_NONE);

	if ((code = check_connected_egg(env, p)))
		return (code);


	fprintf(stderr, "[CLIENT AUTH] Client %d authenticated properly as a player of team {%s}\n", *p->connection, team->name);

	remove_pending_player(env, p);

	return (ERR_NONE);
}

static uint8_t	auth_get_team_name(t_env *env, t_player *p)
{
	t_team	*t;
	uint8_t	code;

	if (strcmp(env->buffers.request, "GRAPHICAL\n") == 0)
	{
		if ((code = handle_graphical_connection(env, p)) != ERR_NONE
			|| (code = remove_pending_player(env, p)) != ERR_NONE)
			return (code);
		return (ERR_NONE);
	}

	PUTTIME()
	fprintf(stderr, "[CLIENT_SENT_TEAM_NAME] Pending client %d sent |%s| as a team name\n", p->pid, env->buffers.request);
	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		if (strcmp(env->buffers.request, t->name) == 0)
		{
			p->team = i;
			p->auth_step++;
			fprintf(stderr, "[TEAM_SLOT_VERIFICATION] Checking team |%s| for a free connection slot (%d)\n", t->name, t->max_client - t->connected);
			if (t->max_client - t->connected > 0)
				return (auth_granting(env, p));
			else
			{
				close(*p->connection);
				*p->connection = -1;
				remove_pending_player(env, p);
				return (send_response(env, p, "No more connection slot available in this team.\n"));
			}
		}
	}
	FLUSH_RESPONSE
	strcat(env->buffers.response, "Invalid team name.\n");
	response(env, p);
	return (kill_player(env, p, true));
}

uint8_t	auth(t_env *env, t_player *p)
{
	uint8_t	(*auth_funcs[AS_MAX])(t_env *env, t_player *p) = {
										[AS_TEAM_NAME] = auth_get_team_name,
										[AS_GRANTING] = auth_granting};

	return (auth_funcs[(int)p->auth_step](env, p));
}

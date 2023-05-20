#include "main.h"

uint8_t	auth_send_welcome(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	FLUSH_RESPONSE
	ft_strcat(env->buffers.response, "BIENVENUE\n");
	response(env, p);
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	build_granting_response(t_env *env, char *response, t_team *team, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	char	*nb_client, *x, *y;

	if (!(nb_client = ft_itoa(team->max_client - team->connected))
		|| !(x = ft_itoa((int)p->tile_x))
		|| !(y = ft_itoa((int)p->tile_y)))
		return (ERR_MALLOC_FAILED);

	FLUSH_RESPONSE
	ft_strcat(response, nb_client);
	ft_strcat(response, "\n");
	ft_strcat(response, x);
	ft_strcat(response, " ");
	ft_strcat(response, y);
	ft_strcat(response, "\n");

	free(nb_client);
	free(x);
	free(y);

	return (ERR_NONE);
}

static void		remove_pending_player(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	t_team		*pending;
	t_player	*tmp;

	pending = &env->world.pending;
	for (int i = 0; i < pending->players.nb_cells; i++)
	{
		tmp = dyacc(&pending->players, i);
		if (tmp->connection == p->connection)
		{
			extract_dynarray(&pending->players, i);
			return ;
		}
	}
	assert(false);
}

static uint8_t	auth_granting(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	t_team	*team;
	uint8_t	code;

	team = dyacc(&env->world.teams, (int)p->team);

	if ((code = build_granting_response(env, env->buffers.response, team, p)))
		return (code);

	response(env, p);
	p->auth_step++;

	if ((team->players.byte_size == 0 && init_dynarray(&team->players, sizeof(t_player), team->max_client))
		|| push_dynarray(&team->players, p, false))
		return (ERR_MALLOC_FAILED);

	remove_pending_player(env, p);

	team->connected++;

	return (ERR_NONE);
}

static uint8_t	auth_get_team_name(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	t_team	*t;
	(void)env;
	(void)p;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		if (strcmp(env->buffers.request, t->name) == 0)
		{
			printf("team ok\n");
			fflush(stdout);
			//sleep(1);

			p->team = i;
			p->auth_step++;
			if (t->max_client - t->connected > 0)
				return (auth_granting(env, p));
			else
			{
				printf("NO MORE SLOTS AVAILABLE\n");
				fflush(stdout);
				sleep(10);
				return (ERR_NONE);
			}

		}
	}
	return (ERR_NONE);
}

uint8_t	auth(t_env *env, t_player *p)
{
	printf("%s\n", __FUNCTION__);
	uint8_t	(*auth_funcs[AS_MAX])(t_env *env, t_player *p) = {
										[AS_TEAM_NAME] = auth_get_team_name,
										[AS_GRANTING] = auth_granting};

	return (auth_funcs[(int)p->auth_step](env, p));
}

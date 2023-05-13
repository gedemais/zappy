#include "main.h"

uint8_t	auth_send_welcome(t_env *env, t_player *p)
{
	FLUSH_RESPONSE
	ft_strcat(env->buffers.response, "BIENVENUE\n");
	response(env, p);
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	auth_get_team_name(t_env *env, t_player *p)
{
	t_team	*t;
	(void)env;
	(void)p;

	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		if (strcmp(env->buffers.request, t->name) == 0)
		{
			p->auth_step++;
			printf("team ok\n");
			fflush(stdout);
			sleep(1);
			return (ERR_NONE);
		}
	}
	return (ERR_NONE);
}

static uint8_t	auth_granting(t_env *env, t_player *p)
{
	t_team	*team;
	char	*nb_client, *x, *y;

	team = dyacc(&env->world.teams, (int)p->team);
	if (!(nb_client = ft_itoa(team->max_client - team->connected))
		|| !(x = ft_itoa((int)p->tile_x))
		|| !(y = ft_itoa((int)p->tile_y)))
		return (ERR_MALLOC_FAILED);

	printf("THERE %d\n", team->max_client);
	fflush(stdout);
	sleep(1);
	FLUSH_RESPONSE

	ft_strcat(env->buffers.response, nb_client);
	ft_strcat(env->buffers.response, "\n");
	ft_strcat(env->buffers.response, x);
	ft_strcat(env->buffers.response, " ");
	ft_strcat(env->buffers.response, y);
	ft_strcat(env->buffers.response, "\n");

	free(nb_client);
	free(x);
	free(y);

	response(env, p);
	p->auth_step++;
	return (ERR_NONE);
}

uint8_t	auth_processing(t_env *env, t_player *p)
{
	uint8_t	(*auth_funcs[AS_MAX])(t_env *env, t_player *p) = {
										[AS_TEAM_NAME] = auth_get_team_name,
										[AS_GRANTING] = auth_granting};

	return (auth_funcs[(int)p->auth_step](env, p));
}

uint8_t	auth(t_env *env, t_player *p)
{
	uint8_t		code;


	if (p->auth_step == AS_MAX)
	{
	//	if (push_dynarray(&t->players, p, false))
	//		return (ERR_MALLOC_FAILED);
	//	extract_dynarray(&env->world.pending.players, i);
		printf("AUTH PROCEDURE SUCCEEDED !\n");
		fflush(stdout);
		sleep(1);
	}
	else if ((code = auth_processing(env, p)))
		return (code);
	return (ERR_NONE);
}

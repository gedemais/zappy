#include "main.h"

static uint8_t	auth_send_welcome(t_env *env, t_player *p)
{
	p->auth_step++;
	FLUSH_RESPONSE
	ft_strcat(env->buffers.response, "BIENVENUE\n");
	response(env, p);
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	auth_get_team_name(t_env *env, t_player *p)
{
	(void)env;
	(void)p;
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	auth_send_nb_client(t_env *env, t_player *p)
{
	(void)env;
	(void)p;
	p->auth_step++;
	return (ERR_NONE);
}

static uint8_t	auth_send_x_y(t_env *env, t_player *p)
{
	(void)env;
	(void)p;
	p->auth_step++;
	return (ERR_NONE);
}

uint8_t	auth_processing(t_env *env, t_player *p)
{
	uint8_t	(*auth_funcs[AS_MAX])(t_env *env, t_player *p) = {
										[AS_WELCOME] = auth_send_welcome,
										[AS_TEAM_NAME] = auth_get_team_name,
										[AS_NB_CLIENT] = auth_send_nb_client,
										[AS_X_Y] = auth_send_x_y};

	return (auth_funcs[(int)p->auth_step](env, p));
}

uint8_t	auth(t_env *env)
{
	t_team		*t;
	t_player	*p;
	uint8_t		code;
	int			team_index = 0;
	int			i = 0;

	return (ERR_NONE);
	while (i < env->world.pending.players.nb_cells)
	{
		p = dyacc(&env->world.pending.players, i);
		if (p->auth_step == AS_MAX)
		{
			// Remove player from pending team and put it in its new team
			team_index = 0;
			t = dyacc(&env->world.teams, team_index);
			if (push_dynarray(&t->players, p, false))
				return (ERR_MALLOC_FAILED);
			extract_dynarray(&env->world.pending.players, i);
			i = 0;
		}
		else if ((code = auth_processing(env, p)))
			return (code);
		i++;
	}
	return (ERR_NONE);
}

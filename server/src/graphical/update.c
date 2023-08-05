#include "main.h"

static uint8_t	already_connected(t_env *env, t_player *p)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, "graphical client already connected.\n");
	return (response(env, p));
}

uint8_t	handle_graphical_connection(t_env *env, t_player *p)
{
	if (env->graphical.connection != NULL)
		return (already_connected(env, p));

	memcpy(&env->graphical, p, sizeof(t_player));

	env->graphical.team = 1;
	return (send_graphical_data(env));
}

#include "main.h"

uint8_t	response(t_env *env, t_player *p)
{
	write(*p->connection, env->buffers.response, strlen(env->buffers.response));
	return (ERR_NONE);
}

uint8_t	gresponse(t_env *env)
{
	write(&env->graphical.connection, env->buffers.gresponse, strlen(env->buffers.gresponse));
	return (ERR_NONE);
}

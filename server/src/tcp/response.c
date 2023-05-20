#include "main.h"

uint8_t	response(t_env *env, t_player *p)
{
	(void)env;
	(void)p;
	write(p->connection, env->buffers.response, strlen(env->buffers.response));
	//printf("|%s| sent to player %p\n", env->buffers.response, p);
	return (ERR_NONE);
}

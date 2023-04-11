#include "main.h"

uint8_t	response(t_env *env, t_player *p)
{
	(void)env;
	(void)p;
	//assert(p->connection > 2);
	printf("%d\n", env->buffers.connections[p->connection]);
	sleep(1);
	write(env->buffers.connections[p->connection], env->buffers.response, strlen(env->buffers.response));
	//printf("|%s| sent to player %p\n", env->buffers.response, p);
	return (ERR_NONE);
}

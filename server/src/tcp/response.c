#include "main.h"

uint8_t	response(t_env *env, t_player *p)
{

	// LOGGING
	PUTTIME()
	fprintf(stderr, "[RESPONSE] Server sent this message to client %d : {%.*s}\n", *p->connection, (int)strlen(env->buffers.response) - 1, env->buffers.response);

	write(*p->connection, env->buffers.response, strlen(env->buffers.response));
	//printf("|%s| sent to player %p\n", env->buffers.response, p);
	return (ERR_NONE);
}

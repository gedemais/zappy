#include "main.h"

uint8_t	send_response(t_env *env, t_player *p, char *s)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, s);
	return (response(env, p));
}

uint8_t	response(t_env *env, t_player *p)
{
	// LOGGING
	PUTTIME()
	fprintf(stderr, "[RESPONSE] Server sent this message to client %d : {%.*s}\n", *p->connection, (int)strlen(env->buffers.response) - 1, env->buffers.response);

	write(*p->connection, env->buffers.response, strlen(env->buffers.response));
	return (ERR_NONE);
}

uint8_t	gresponse(t_env *env)
{
	printf("%s\n", env->buffers.gresponse);
	fflush(stdout);

	if (env->graphical.connection)
		write(*env->graphical.connection, env->buffers.gresponse, strlen(env->buffers.gresponse));

	//sleep(1);
	FLUSH_GRESPONSE
	return (ERR_NONE);
}

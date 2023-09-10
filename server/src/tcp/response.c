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
	//sleep(1);
	return (ERR_NONE);
}

uint8_t	gresponse(t_env *env)
{

	if (env->graphical.team != 0)
	{
		//printf("%s\n", env->buffers.gresponse);
		//fflush(stdout);
		write(*env->graphical.connection, env->buffers.gresponse, strlen(env->buffers.gresponse));
		//usleep(500000);
	}

	FLUSH_GRESPONSE
	return (ERR_NONE);
}

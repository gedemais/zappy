#include "main.h"

// ASCII art drawing at server launch
static void			intro(void)
{
	char art[1024] = "===================================\n  ______                       \n |___  /                       \n    / / __ _ _ __  _ __  _   _ \n   / / / _` | '_ \\| '_ \\| | | |\n  / /_| (_| | |_) | |_) | |_| |\n /_____\\__,_| .__/| .__/ \\__, |\n            | |   | |     __/ |\n            |_|   |_|    |___/ \n===================================\n";

	write(1, art, strlen(art));
}

// Allocation of buffers for the server
static uint8_t		init_buffers(t_buffers *buffers)
{
	if (!(buffers->response = (char*)malloc(sizeof(char) * RESPONSE_SIZE))
		|| !(buffers->gresponse = (char*)malloc(sizeof(char) * RESPONSE_SIZE)))
		return (ERR_MALLOC_FAILED);

	memset(buffers->response, 0, sizeof(char) * RESPONSE_SIZE);

	if (dynarray_init(&buffers->view, sizeof(t_tile), 64))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

uint8_t	init_server(t_env *env, int argc, char **argv)
{
	uint8_t	code;

	intro();

	if ((code = parse_options(env, argc, argv)) != ERR_NONE
		|| (code = init_buffers(&env->buffers))
		|| (code = init_world(env)) != ERR_NONE
		|| (code = init_tcp(env)))
		return (code);

	env->settings.tick_length = 1000000 / env->settings.t; // Duration of each tick in milliseconds

	return (ERR_NONE);
}

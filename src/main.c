#include "main.h"

static void			intro(void)
{
	char art[1024] = "===================================\n  ______                       \n |___  /                       \n    / / __ _ _ __  _ __  _   _ \n   / / / _` | '_ \\| '_ \\| | | |\n  / /_| (_| | |_) | |_) | |_| |\n /_____\\__,_| .__/| .__/ \\__, |\n            | |   | |     __/ |\n            |_|   |_|    |___/ \n===================================\n";

	write(1, art, strlen(art));
}

static uint8_t		init_buffers(t_buffers *buffers)
{
	if (!(buffers->response = (char*)malloc(sizeof(char) * RESPONSE_SIZE)))
		return (ERR_MALLOC_FAILED);

	memset(buffers->response, 0, sizeof(char) * RESPONSE_SIZE);

	if (init_dynarray(&buffers->view, sizeof(t_tile), 64))
		return (ERR_MALLOC_FAILED);

	return (ERR_NONE);
}

static uint8_t		zappy_server(t_env *env, int argc, char **argv)
{
	bool	run = true;
	uint8_t	code;

	if ((code = parse_options(env, argc, argv)) != ERR_NONE
		|| (code = init_buffers(&env->buffers))
		|| (code = init_world(env)) != ERR_NONE)
		return (code);


	add_player(env, dyacc(&env->world.teams, 0));
	t_team *a = dyacc(&env->world.teams, 0);
	t_player *pa = dyacc(&a->players, 0);
	cmd_see(env, pa);
	cmd_inventory(env, pa);
	print_map(env);
	while (run)
	{
		if ((code = tick(env)))
			return (code);
		break;
	}

	return (ERR_NONE);
}

int		main(int argc, char **argv)
{
	t_env			env;
	unsigned char	err_code;

	srand(time(NULL));
	memset(&env, 0, sizeof(t_env));

	intro();

	if ((err_code = zappy_server(&env, argc, argv)) != ERR_NONE)
		return (error(&env, err_code));

	free_env(&env);
	return (0);
}

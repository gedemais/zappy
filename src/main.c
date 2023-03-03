#include "main.h"

static void			intro(void)
{
	char art[1024] = "===================================\n  ______                       \n |___  /                       \n    / / __ _ _ __  _ __  _   _ \n   / / / _` | '_ \\| '_ \\| | | |\n  / /_| (_| | |_) | |_) | |_| |\n /_____\\__,_| .__/| .__/ \\__, |\n            | |   | |     __/ |\n            |_|   |_|    |___/ \n===================================\n";

	write(1, art, strlen(art));
}

static uint8_t		zappy_server(t_env *env, int argc, char **argv)
{
	bool	run = true;
	uint8_t	code;

	if ((code = parse_options(env, argc, argv)) != ERR_NONE)
		return (code);

	while (run)
	{
		break;
	}
	return (ERR_NONE);
}

int		main(int argc, char **argv)
{
	t_env			env;
	unsigned char	err_code;

	memset(&env, 0, sizeof(t_env));

	intro();

	if ((err_code = zappy_server(&env, argc, argv)) != ERR_NONE)
		return (error(&env, err_code));

	free_env(&env);
	return (0);
}

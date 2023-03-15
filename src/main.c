#include "main.h"

static uint8_t		zappy_server(t_env *env, int argc, char **argv)
{
	bool	run = true;
	uint8_t	code;

	if ((code = init_server(env, argc, argv)))
		return (code);

// tmp
	add_player(env, dyacc(&env->world.teams, 0));
	add_player(env, dyacc(&env->world.teams, 1));
// tmp

	while (run)
	{
		print_map(env);
		if ((code = tick(env)))
			return (code);
	}

	return (ERR_NONE);
}

int		main(int argc, char **argv)
{
	t_env			env;
	unsigned char	err_code;

	srand(time(NULL));
	memset(&env, 0, sizeof(t_env));

	if ((err_code = zappy_server(&env, argc, argv)) != ERR_NONE)
		return (error(&env, err_code));

	free_env(&env);
	return (0);
}

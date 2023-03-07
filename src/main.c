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

	if ((code = parse_options(env, argc, argv)) != ERR_NONE
		|| (code = init_world(env)) != ERR_NONE)
		return (code);


	add_player(env, dyacc(&env->world.teams, 0));

//	add_player(env, dyacc(&env->world.teams, 1));

	t_team *a = dyacc(&env->world.teams, 0);
//	t_team *b = dyacc(&env->world.teams, 1);

	t_player *pa = dyacc(&a->players, 0);
//	t_player *pb = dyacc(&b->players, 0);

	cmd_see(env, pa);
//	cmd_see(env, pb);

	print_map(env);
	while (run)
		if ((code = tick(env)))
			return (code);

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

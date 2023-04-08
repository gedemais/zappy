#include "main.h"

static uint8_t		zappy_server(t_env *env, int argc, char **argv)
{
	bool	run = true;
	uint8_t	code;

	if ((code = init_server(env, argc, argv)))
		return (code);


	add_player(env, (t_team*)env->world.teams.c, 0);
	while (run)
	{
		if (MAP_PRINT)
		{
			write(1, "\033[2J", 1);
			print_map(env);
		}
		if (DATA_PRINT)
			printf("DATA\n");
		fflush(stdout);
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

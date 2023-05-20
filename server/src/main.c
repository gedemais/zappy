#include "main.h"

static uint8_t		zappy_server(t_env *env, int argc, char **argv)
{
	bool	run = true;
	uint8_t	code;

	// Server initialisation
	if ((code = init_server(env, argc, argv)))
		return (code);

	//tmp
	//add_player(env, (t_team*)env->world.teams.c, 0);

	// Infinite loop running the server
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

		// Server tasks
		if ((code = tick(env)))
			return (code);
	}

	return (ERR_NONE);
}

int		main(int argc, char **argv)
{
	t_env			env;
	unsigned char	err_code;

	srand(time(NULL)); // Random seed initialisation
	memset(&env, 0, sizeof(t_env)); // Wipe environment structure

	// Main server function
	if ((err_code = zappy_server(&env, argc, argv)) != ERR_NONE)
		return (error(&env, err_code));

	free_env(&env);
	return (0);
}

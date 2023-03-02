#include "main.h"

#define OPT_MAX 6

uint8_t	opt_load_port(t_env *env, char *data)
{
	printf("%s\n", __FUNCTION__);
	if (strlen(data) > 5)
		return (ERR_INVALID_PORT_NUMBER);

	for (uint32_t i = 0; data[i]; i++)
		if (ft_isdigit(data[i]) == 0)
			return (ERR_INVALID_PORT_NUMBER);

	env->settings.port = (uint16_t)ft_atoi(data);
	return (ERR_NONE);
}

uint8_t	opt_load_width(t_env *env, char *data)
{
	printf("%s\n", __FUNCTION__);
	if (strlen(data) > 5)
		return (ERR_INVALID_PORT_NUMBER);

	for (uint32_t i = 0; data[i]; i++)
		if (ft_isdigit(data[i]) == 0)
			return (ERR_INVALID_PORT_NUMBER);

	env->settings.map_width = (uint16_t)ft_atoi(data);
	return (ERR_NONE);
}

uint8_t	opt_load_height(t_env *env, char *data)
{
	printf("%s\n", __FUNCTION__);
	if (strlen(data) > 5)
		return (ERR_INVALID_PORT_NUMBER);

	for (uint32_t i = 0; data[i]; i++)
		if (ft_isdigit(data[i]) == 0)
			return (ERR_INVALID_PORT_NUMBER);

	env->settings.map_height = (uint16_t)ft_atoi(data);
	return (ERR_NONE);
}

uint8_t	opt_load_teams(t_env *env, char *data)
{
	char	**tokens;
	t_team	team;

	if (init_dynarray(&env->world.teams, sizeof(t_team), 8)
		|| !(tokens = ft_strsplit(data, " ")))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; tokens[i]; i++)
	{
		memset(&team, 0, sizeof(t_team));
		if (!(team.name = ft_strdup(tokens[i])))
		{
			ft_free_ctab(tokens);
			return (ERR_MALLOC_FAILED);
		}

		if (push_dynarray(&env->world.teams, &team, false))
		{
			free(team.name);
			ft_free_ctab(tokens);
			return (ERR_MALLOC_FAILED);
		}
	}

	ft_free_ctab(tokens);
	return (ERR_NONE);
}

/*
uint8_t	opt_load_(t_env *env, char *data)
{
	return (ERR_NONE);
}

uint8_t	opt_load_(t_env *env, char *data)
{
	return (ERR_NONE);
}*/

uint8_t	dispatcher(t_env *env, char opt, char *data)
{
	uint8_t	(*opt_loaders[OPT_MAX])(t_env *env, char *data) = {
		opt_load_port,
		opt_load_width,
		opt_load_height,
		opt_load_teams,
		NULL,
		NULL
	};
	char	options[OPT_MAX] = "pxynct";
	uint8_t	code;


	for (uint32_t i = 0; i < OPT_MAX; i++)
		if (opt == options[i])
			if ((code = opt_loaders[i](env, data)))
				return (code);

	return (ERR_NONE);
}

uint8_t	parse_options(t_env *env, int argc, char **argv)
{
	uint8_t	code;
	int		opt;

	while ((opt = getopt(argc, argv, ":p:x:y:n:c:t:")) != -1)
	{

		printf("%c|%s\n", opt, optarg);

		if (opt == ':')
			return (ERR_MISSING_ARGUMENT);
		else if (opt == '?')
			return (ERR_INVALID_OPT);
		else if ((code = dispatcher(env, opt, optarg))) // passer l'adresse de optarg pour pouvoir itérer sur argv dans les loaders avec optind
			return (code);
	}

	        optind--;
        for( ;optind < argc && *argv[optind] != '-'; optind++){
              DoSomething( argv[optind] );         
        }

	// Checks
	printf("port : %d\n", env->settings.port);
	printf("x : %d\n", env->settings.map_width);
	printf("y : %d\n", env->settings.map_height);
	printf("nb_teams : %d\n", env->world.teams.nb_cells);
	printf("max_connections : %d\n", env->settings.max_connections);
	printf("t : %d\n", env->settings.t);
	
	t_team	*t;
	for (int i = 0; i < env->world.teams.nb_cells; i++)
	{
		t = dyacc(&env->world.teams, i);
		printf("%s\n", t->name);
	}
	return (ERR_NONE);
}

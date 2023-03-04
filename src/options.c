#include "main.h"

#define OPT_MAX 6

static bool	is_valid_number(char *nbr, size_t max_len)
{
	if (strlen(nbr) > max_len)
		return (false);

	for (uint32_t i = 0; nbr[i]; i++)
		if (ft_isdigit(nbr[i]) == 0)
			return (false);

	return (true);
}

static uint8_t	opt_load_port(t_env *env, char **args)
{
	//printf("%s\n", __FUNCTION__);

	if (is_valid_number(*args, 5) == false)
		return (ERR_INVALID_PORT_NUMBER);

	env->settings.port = (uint16_t)ft_atoi(*args);
	return (ERR_NONE);
}

static uint8_t	opt_load_width(t_env *env, char **args)
{
	if (is_valid_number(*args, 5) == false)
		return (ERR_INVALID_MAP_DIMS);

	env->settings.map_width = (uint16_t)ft_atoi((*args));
	//printf("%s succeeded\n", __FUNCTION__);
	return (ERR_NONE);
}

static uint8_t	opt_load_height(t_env *env, char **args)
{
	if (is_valid_number(*args, 5) == false)
		return (ERR_INVALID_MAP_DIMS);

	env->settings.map_height = (uint16_t)ft_atoi((*args));
	//printf("%s succeeded\n", __FUNCTION__);
	return (ERR_NONE);
}

static uint8_t	opt_load_teams(t_env *env, char **args)
{
	t_team	team;

	//printf("%s succeeded\n", __FUNCTION__);
	if (init_dynarray(&env->world.teams, sizeof(t_team), 8))
		return (ERR_MALLOC_FAILED);

	for (uint32_t i = 0; args[i] && args[i][0] != '-'; i++)
	{
		printf("%s\n", args[i]);
		memset(&team, 0, sizeof(t_team));
		if (!(team.name = ft_strdup(args[i])))
			return (ERR_MALLOC_FAILED);

		if (push_dynarray(&env->world.teams, &team, false))
		{
			free(team.name);
			return (ERR_MALLOC_FAILED);
		}
	}
	return (ERR_NONE);
}


static uint8_t	opt_load_c(t_env *env, char **args)
{
	//printf("%s succeeded\n", __FUNCTION__);
	if (is_valid_number(*args, 5) == false)
		return (ERR_INVALID_CONNECTION_ARG);

	env->settings.max_connections = ft_atoi(*args);
	return (ERR_NONE);
}


static uint8_t	opt_load_t(t_env *env, char **args)
{
	if (is_valid_number(*args, 5) == false)
		return (ERR_INVALID_TIME_SETTING);

	env->settings.t = ft_atoi(*args);
	//printf("%s succeeded\n", __FUNCTION__);
	return (ERR_NONE);
}

static uint8_t	dispatcher(t_env *env, char opt, char **args, bool filled[OPT_MAX])
{
	uint8_t	(*opt_loaders[OPT_MAX])(t_env *env, char **args) = {
		opt_load_port,
		opt_load_width,
		opt_load_height,
		opt_load_teams,
		opt_load_c,
		opt_load_t
	};
	char	options[OPT_MAX] = "pxynct";
	uint8_t	code;
	bool	match;

	for (uint32_t i = 0; i < OPT_MAX; i++)
	{
		match = (bool)(opt == options[i]);
		if (match && (code = opt_loaders[i](env, args)) != ERR_NONE)
			return (code);
		else if (match && filled[i] == false)
			filled[i] = true;
		else if (match)
			return (ERR_DUPLICATE_OPT);
	}

	return (ERR_NONE);
}

uint8_t	parse_options(t_env *env, int argc, char **argv)
{
	bool		filled[OPT_MAX] = {};
	uint8_t		code;
	int			opt;

	while ((opt = getopt(argc, argv, ":p:x:y:n:c:t:")) != -1)
	{
		if (opt == ':')
			return (ERR_MISSING_ARGUMENT);
		else if (opt == '?')
			return (ERR_INVALID_OPT);
		else if ((code = dispatcher(env, opt, &argv[optind - 1], filled))) // passer l'adresse de argv pour pouvoir itérer sur argv dans les loaders avec optind
			return (code);
	}

	for (uint32_t i = 0; i < OPT_MAX; i++)
		if (filled[i] == false)
			return (ERR_MISSING_OPT);

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

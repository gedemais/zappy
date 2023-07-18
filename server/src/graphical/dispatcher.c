#include "main.h"

uint8_t	load_cmd_params(t_env *env, char **tokens, unsigned int cmd)
{
	int		value;

	if (cmd == GCMD_PLAYER_POSITION || cmd == GCMD_PLAYER_LEVEL || cmd == GCMD_PLAYER_INVENTORY)
	{
		if (ft_arrlen(tokens) != 2 || tokens[1][0] == '#')
			return (1);

		if (is_only_number(&tokens[1][1]))
			return (1);

		env->gpid = atoi(&tokens[1][1]);
	}
	else if (cmd == GCMD_BLOCK_CONTENT)
	{
		if (ft_arrlen(tokens) != 3)
			return (1);

		if ((value = atoi(tokens[1])) < 0 || value >= env->settings.map_width)
			return (1);
		env->gx = value;

		if ((value = atoi(tokens[1])) < 0 || value >= env->settings.map_width)
			return (1);
		env->gy = value;
	}
	else if (cmd == GCMD_SST)
	{
		if (ft_arrlen(tokens) != 2 || is_only_number(tokens[1]) == false)
			return (1);

		if ((value = atoi(tokens[1])) < 0 || value > MAX_T)
			return (1);

		env->gnew_t = value;
	}

	return (0);
}

uint8_t	graphical_request(t_env *env)
{
	char			**tokens;
	uint8_t			gcode;

	if (!(tokens = ft_strsplit(env->buffers.request, " ")))
		return (ERR_MALLOC_FAILED);

	for (unsigned int i = 0; i < GCMD_MAX; i++)
	{
		if (strcmp(gcmd_names[i], tokens[0]) == 0)
		{
			if ((gcode = load_cmd_params(env, tokens, i)) != 0)
			{
				FLUSH_RESPONSE
				strcat(env->buffers.response, "sbp\n");
				ft_arrfree(tokens);
				return (response(env, &env->gplayer));
			}

			if ((gcode = gcmds[i](env)))
				return 

			response(env, &env->graphical);
		}
	}
	FLUSH_RESPONSE
	strcat(env->buffers.response, "suc\n");
	ft_arrfree(tokens);
	return (response(env, &env->gplayer));
}


#include "main.h"

uint8_t	waiting_response(t_env *env, t_player *player)
{
	FLUSH_RESPONSE
	strcat(env->buffers.response, "You have to wait for the game to start.\n");
	return (response(env, player));
}

uint8_t	place_command_in_queue(t_env *env, t_player *player)
{
	t_cmd			new;
	char			**lines;
	char			**tokens;
	bool			req;
	bool			cmd_found;
	uint8_t			code;

	if (!(lines = ft_strsplit(env->buffers.request, "\n")))
		return (ERR_MALLOC_FAILED);

	for (uint32_t line = 0; lines[line]; line++)
	{
		cmd_found = false;
		if (strlen(lines[line]) <= 1)
			continue ;

		if (!(tokens = ft_strsplit(lines[line], " ")))
		{
			ft_arrfree(lines);
			return (ERR_MALLOC_FAILED);
		}

		for (int i = 0; i < CMD_MAX; i++)
		{
			if (strcmp(tokens[0], cmd_names[i]) == 0)
			{
				cmd_found = true;

				req = false;
				if ((code = check_requirements(env, tokens, player, i, &req)))
					return (code);

				if (req == false)
				{
					FLUSH_RESPONSE
					ft_arrfree(lines);
					ft_arrfree(tokens);
					send_ko(env, player);
					break ;
				}

				if (player->cmd_queue.nb_cells >= MAX_QUEUED_CMD)
					break ;

				bzero(&new, sizeof(t_cmd));
				new = commands[i];
				new.tokens = tokens;
				new.id = rand() * rand() * rand();
				new.id *= (new.id >= 0) ? 1 : -1;

				if ((player->cmd_queue.byte_size == 0
					&& dynarray_init(&player->cmd_queue, sizeof(t_cmd), 10))
					|| dynarray_push(&player->cmd_queue, &new, false))
				{
					ft_arrfree(lines);
					ft_arrfree(tokens);
					return (ERR_MALLOC_FAILED);
				}

				// LOGGING
				PUTTIME()
				fprintf(stderr, "[COMMAND STORAGE] message : {%s} from client %d have been stored as command %d\n", lines[line], *player->connection, new.id);
				break;
			}
		}

		if (!cmd_found)
		{
			fprintf(stderr, "|||%s|||\n", tokens[0]);
			ft_arrfree(tokens);
			ft_arrfree(lines);
			return (ERR_CMD_NOT_FOUND);
		}
	}

	ft_arrfree(lines);
	return (ERR_NONE);
}

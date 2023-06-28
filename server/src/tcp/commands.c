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
	bool			cmd_found;

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
			if (strcmp(tokens[1], cmd_names[i]) == 0)
			{
				cmd_found = true;

				if (player->cmd_queue.nb_cells >= MAX_QUEUED_CMD)
					break ;

				bzero(&new, sizeof(t_cmd));
				new = commands[i];
				new.tokens = tokens;
				new.id = atoi(tokens[0]);

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

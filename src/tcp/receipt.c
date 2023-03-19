#include "main.h"
/*
uint8_t	connections_receipt(t_env *env)
{
	return (ERR_NONE);
}

uint8_t	commands_receipt(t_env *env)
{
	return (ERR_NONE);
}*/

uint8_t	place_command_in_queue(t_env *env)
{
	t_cmd	new;
	char	**lines;
	char	**tokens;
	bool	cmd_found;

	if (!(lines = ft_strsplit(env->buffers.request, "\n")))
		return (ERR_MALLOC_FAILED);

	for (uint32_t line = 0; lines[line]; line++)
	{
		cmd_found = false;
		if (strlen(lines[line]) <= 1)
			continue ;

		if (!(tokens = ft_strsplit(lines[line], " ")))
			return (ERR_MALLOC_FAILED);

		for (int i = 0; i < CMD_MAX; i++)
		{
			if (strcmp(tokens[0], cmd_names[i]) == 0)
			{
				bzero(&new, sizeof(t_cmd));
				new = commands[i];
				new.tokens = tokens;
				// new.player = player;
				printf("%s command received (%d commands in queue)\n", tokens[0], env->buffers.cmd_queue.nb_cells);
				fflush(stdout);
				if (push_dynarray(&env->buffers.cmd_queue, &new, false))
					return (ERR_MALLOC_FAILED);
				cmd_found = true;
				break; // not enough
			}
		}

		if (!cmd_found)
		{
			ft_free_ctab(lines);
			return (ERR_CMD_NOT_FOUND);
		}
	}

	ft_free_ctab(lines);
	return (ERR_NONE); // Send response to client ?
}

uint8_t	receipt(t_env *env)
{
	fd_set				read_fd_set;
	struct sockaddr_in	new_addr;
	struct timeval		timeout = {.tv_sec = 0, .tv_usec = env->settings.tick_length * (1.0f / env->settings.t)};
	socklen_t			addrlen = sizeof(new_addr);
	int					*connections = env->buffers.connections;
	int					ret, new_fd;
	uint8_t				code;

	FD_ZERO(&read_fd_set);

	int	sets = 0;
	for (uint32_t i = 0; i < env->settings.max_connections; i++)
		if (connections[i] >= 0)
		{
			FD_SET(connections[i], &read_fd_set);
			sets++;
		}


	if ((ret = select(env->settings.max_connections + 1, &read_fd_set, NULL, NULL, &timeout)) >= 0) // If select does not fail
	{
		if (FD_ISSET(env->tcp.server_fd, &read_fd_set))
		{
			new_fd = accept(env->tcp.server_fd, (struct sockaddr*)&new_addr, &addrlen);
			if (new_fd >= 0) // && at least one connection still available in one team
			{
				for (uint32_t i = 0; i < env->settings.max_connections; i++)
					if (connections[i] < 0)
					{
						printf("New client connected\n");
						// add pending player and start auth procedure
						connections[i] = new_fd;
						break;
					}
			}
			else
				return (ERR_ACCEPT_FAILED);
		}

		for (uint32_t i = 1; i < env->settings.max_connections; i++)
		{
			if (connections[i] >= 0 && FD_ISSET(connections[i], &read_fd_set))
			{
				/*if (ret == 0) // Connection closes
				{
					printf("Client disconnected\n");
					close(connections[i]);
					connections[i] = -1;
				}*/
				if (ret > 0)
				{
					ret = recv(connections[i], env->buffers.request, REQUEST_BUFF_SIZE, 0);
					env->buffers.request[ret] = '\0';
					if ((code = place_command_in_queue(env)))
						return (code);
				}
				else if (ret == -1)
					return (ERR_RECV_FAILED);
			}
		}
	}

	return (ERR_NONE);
}

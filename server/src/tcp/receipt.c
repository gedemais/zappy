#include "main.h"

uint8_t	connections_receipt(t_env *env, fd_set *read_fd_set, struct sockaddr_in *new_addr, socklen_t *addrlen)
{
	int			new_fd;
	uint8_t		code;

	if (FD_ISSET(env->tcp.server_fd, read_fd_set)) // If a new event occured on the server
	{
		new_fd = accept(env->tcp.server_fd, (struct sockaddr*)new_addr, addrlen); // We try to accept a connection
		if (new_fd >= 0) // && at least one connection still available in one team (we can also check it with a 'found' boolean set by the for loop before the break)
		{
			for (uint32_t i = 0; i < 1024; i++) // Looking for a connection slot
				if (env->buffers.connections[i] < 0) // If the slot is available
				{
					printf("New client connected on slot %d (fd : %d)\n", i, new_fd);
					fflush(stdout);
					//sleep(1);

					env->buffers.connections[i] = new_fd; // We save the connection for later
														  //
					add_player(env, &env->world.pending, &env->buffers.connections[i]); // We add a new player in the pending players list

					if ((code = auth_send_welcome(env, (t_player*)dyacc(&env->world.pending.players, env->world.pending.players.nb_cells - 1))))
						return (code);

					break;
				}
		}
		else
			return (ERR_ACCEPT_FAILED);

	}
	return (ERR_NONE);
}

static uint8_t	process_request(t_env *env, int client_fd)
{
	t_player	*p;
	uint8_t		code;

	if (client_fd == *env->gplayer.connection && (code = graphical_request(env)) != ERR_NONE)
		return (code);
	else if ((p = get_pending_client(env, client_fd)) != NULL)
		return (auth(env, p));
	else if ((p = get_team_client(env, client_fd)))
		return (env->start ? place_command_in_queue(env, p) : waiting_response(env, p));

	return (ERR_NONE);
}

uint8_t	receipt(t_env *env)
{
	fd_set				read_fd_set;
	struct sockaddr_in	new_addr;
	struct timeval		timeout = {.tv_sec = 0, .tv_usec = env->settings.tick_length * (1.0f / env->settings.t)};
	socklen_t			addrlen = sizeof(new_addr);
	int					*connections = env->buffers.connections;
	int					ret;
	uint8_t				code;

	FD_ZERO(&read_fd_set);

	int	sets = 0;
	for (uint32_t i = 0; i < 1024; i++)
		if (connections[i] >= 0)
		{
			FD_SET(connections[i], &read_fd_set);
			sets++;
		}

	if ((ret = select(1024, &read_fd_set, NULL, NULL, &timeout)) >= 0) // If select does not fail
	{

		if ((code = connections_receipt(env, &read_fd_set, &new_addr, &addrlen)) != ERR_NONE)
			return (code);

		for (uint32_t i = 1; i < 1024; i++) // Starting from 1 because the first fd represents the server's connection
		{
			if (connections[i] >= 0 && FD_ISSET(connections[i], &read_fd_set)) // If a player is connected on this slot, and an event occured on it
			{
				if (ret > 0)
				{
					if ((ret = recv(connections[i], env->buffers.request, REQUEST_BUFF_SIZE, 0)) <= 0) // Connection closes
					{
						printf("Client disconnected of slot %d (fd : %d)!\n", i, connections[i]);
						fflush(stdout);
						sleep(1);

						// Remove player from his team
						if ((code = kill_player(env, get_team_client(env, connections[i]), true)))
							return (code);

						continue;
					}
					env->buffers.request[ret] = '\0'; // Segfault after some time...
					//printf("REQUEST : |%s| (%ld bytes)", env->buffers.request, strlen(env->buffers.request)); // Request isn't always received
					//fflush(stdout);

					if ((code = process_request(env, connections[i])))
						return (code);
					//if ((code = place_command_in_queue(env, connections[i])))
					//	return (code);
				}
				else if (ret == -1)
					return (ERR_RECV_FAILED);
			}
		}
	}
	else
		return (ERR_SELECT_FAILED);

	return (ERR_NONE);
}

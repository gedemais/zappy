#include "main.h"

uint8_t	commands_receipt(t_env *env)
{
	fd_set				read_fd_set;
	struct sockaddr_in	new_addr;
	socklen_t			addrlen;
	int					*connections = env->buffers.connections;
	int					ret, new_fd;

	FD_ZERO(&read_fd_set);

	for (uint32_t i = 0; i < env->settings.max_connections; i++)
		if (connections[i] >= 0)
			FD_SET(connections[i], &read_fd_set);

	if ((ret = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL)) >= 0) // If select wakes up
	{
		if (FD_ISSET(env->tcp.server_fd, &read_fd_set)) // 
		{
			new_fd = accept(env->tcp.server_fd, (struct sockaddr*)&new_addr, &addrlen);
			if (new_fd >= 0) // && at least one connection still available
			{
				for (uint32_t i = 0; i < env->settings.max_connections; i++)
					if (connections[i] < 0)
					{
						printf("New client connected\n");
						connections[i] = new_fd;
						break;
					}
			}
			else
				return (ERR_ACCEPT_FAILED);
		}

		printf("Checking for requests...\n");
		for (uint32_t i = 1; i < env->settings.max_connections; i++)
		{
			if (connections[i] > 0 && FD_ISSET(connections[i], &read_fd_set))
			{
				ret = recv(connections[i], env->buffers.request, REQUEST_BUFF_SIZE, 0);
				env->buffers.request[ret] = '\0';

				if (ret == 0) // Connection closes
				{
					printf("Client disconnected\n");
					close(connections[i]);
					connections[i] = -1;
				}
				else if (ret > 0)
					printf("%s\n", env->buffers.request);
				else if (ret == -1)
					return (ERR_RECV_FAILED);

				fflush(stdout);
			}

		}
	}

	return (ERR_NONE);
}

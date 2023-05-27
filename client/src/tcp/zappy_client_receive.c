#include "zappy_client.h"


static int	zappy_handle_server_response(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "zappy_client_receive: response for {%s}: (%s)\n----------\n", client->cmds[client->cmd_idx].cmd, client->buf);
	r = client->cmds[client->cmd_idx].cb(client);
	client->cmd_idx++;
 	client->cmd_idx %= ZAPPY_CLIENT_MAX_STACKED_CMD;
	client->cmd_stack_size--;
	return (r);
}

int			zappy_client_receive(zappy_client_t *client)
{
	int 			r = 0;
	fd_set			read_fd_set;
	struct timeval	timeout = {.tv_sec = 0, .tv_usec = 100};

	FD_ZERO(&read_fd_set);
	FD_SET(client->socket, &read_fd_set);
	if ((r = select(1024 + 1, &read_fd_set, NULL, NULL, &timeout)) >= 0) // If select does not fail
	{
		if (r > 0)
		{
			// Data available
			if (recv(client->socket, client->buf, CLIENT_BUFSIZE, 0) < 0) {
				perror("recv");
				r = -1;
			}
			if (r != -1)
			{
				// =====================================
				// permet de gerer les messages de l'API
				// =====================================
				if (!memcmp(client->buf, "deplacement", strlen("deplacement"))) {
					zappy_deplacement(client);
				}
				else if (!memcmp(client->buf, "message", strlen("message"))) {
					zappy_message(client);
				}
				else if (!memcmp(client->buf, "mort", strlen("mort"))) {
					zappy_mort(client);
				}
				// ======================================
				// permet de gerer les responses de l'API
				// ======================================
				zappy_handle_server_response(client);
			}
		}
		else if (r < 0) {
			r = -1;
		}
	}
	return (r);
}

#include "zappy_client.h"


/* If a msg has been recv return 1 */
static int	zappy_handle_server_message(zappy_client_t *client)
{
	int	r = 0;

	if (!memcmp((char *)client->buf, "deplacement", strlen("deplacement"))) {
		r = zappy_deplacement(client);
		if (r == 0) {
			r = 1;
		}
	}
	else if (!memcmp((char *)client->buf, "message", strlen("message"))) {
		r = zappy_message(client);
		if (r == 0) {
			r = 1;
		}
	}
	else if (!memcmp((char *)client->buf, "mort", strlen("mort"))) {
		r = zappy_mort(client);
		if (r == 0) {
			r = 1;
		}
	}
	return (r);
}

static int	zappy_handle_server_response(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "zappy_client_receive: response for id %d: (%s)\n----------\n", client->cmd_idx, client->buf);
	r = client->cmds[client->cmd_idx].cb(client, &client->cmds[client->cmd_idx]);
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
				r = zappy_handle_server_message(client);
				if (r == 0) {
					r = zappy_handle_server_response(client);
				}
				else if (r == 1)
				{
					r = 0;
				}
			}
		}
		else if (r < 0) {
			r = -1;
		}
	}
	return (r);
}

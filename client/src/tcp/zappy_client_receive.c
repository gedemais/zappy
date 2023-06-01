#include "zappy_client.h"


/* If a msg has been recv return 1 */
static int	zappy_handle_server_message(zappy_client_t *client)
{
	int	r = 0;

	// les fonction handle des messages serveurs renvoient 1 si tout c'est bien passé
	if (!memcmp((char *)client->buf, "deplacement", strlen("deplacement"))) {
		r = zappy_deplacement(client);
	}
	else if (!memcmp((char *)client->buf, "message", strlen("message"))) {
		r = zappy_message(client);
	}
	else if (!memcmp((char *)client->buf, "mort", strlen("mort"))) {
		r = zappy_mort(client);
	}
	return (r);
}

static int	zappy_handle_server_response(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "zappy_client_receive: response for cmd_id: %d: (%s)\n----------\n", client->cmd_idx, client->buf);
	r = client->cmds[client->cmd_idx].cb(client, &client->cmds[client->cmd_idx]);
	client->cmd_idx++;
 	client->cmd_idx %= ZAPPY_CLIENT_MAX_STACKED_CMD;
	client->cmd_stack_size--;
	return (r);
}

int			zappy_client_receive(zappy_client_t *client)
{
	int 			r = 0, buf_len;
	fd_set			read_fd_set;
	struct timeval	timeout = {.tv_sec = 0, .tv_usec = 100};

	FD_ZERO(&read_fd_set);
	FD_SET(client->socket, &read_fd_set);
	if ((r = select(1024, &read_fd_set, NULL, NULL, &timeout)) >= 0)
	{
		if (r > 0)
		{
			// reset du buffer
			bzero(client->buf, CLIENT_BUFSIZE);
			// Data available
			if (recv(client->socket, client->buf, CLIENT_BUFSIZE, 0) < 0) {
				perror("recv");
				r = -1;
			}
			if (r != -1)
			{
				r = zappy_handle_server_message(client);
				// si le serveur envoit un message alors r vaudra 1
				// on n'envoit donc plus aucune query pour ce tour de loop
				buf_len = strlen((char *)client->buf);
				if (r == 0 && buf_len > 0) {
					// on supprime le \n de la reponse du serveur
					client->buf[buf_len - 1] = '\0';
					r = zappy_handle_server_response(client);
				}
				else if (r == 1) { // on remet r à 0 car il n'y a pas d'erreur
					r = 0;
				}
			}
		}
		else if (r < 0) {
			r = -1;
		}
	} else {
		perror("select failed");
	}
	return (r);
}

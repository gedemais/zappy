#include "zappy_client.h"


int			zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb)
{
	int r = 0, cmd_id;

	if (client->cmd_stack_size >= ZAPPY_CLIENT_MAX_STACKED_CMD) {
		r = ZAPPY_WAIT;
	}
	if (r == 0)
	{
		cmd_id = (client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD;
		// on stock l'id de la commande dans la stack : useless pour le moment
		client->cmds[cmd_id].id = cmd_id;
		// on copie la cmd dans un buffer c'est utile pour prendre et déposer afin d'identifier la ressource concernée
		memcpy(client->cmds[cmd_id].str, cmd, CLIENT_BUFSIZE);
		// on associe le cb correspond a la cmd dans la stack
		client->cmds[cmd_id].cb = cb;
		// on augmente la cmd_stack
		++client->cmd_stack_size;
		// on reset le buffer ?
		bzero(client->buf, CLIENT_BUFSIZE); // TODO
		fprintf(stderr, "%s: send (%s) at cmd_id: %d\n----------\n", __func__, cmd, cmd_id);
		if (send(client->socket, cmd, len, 0) < 0) {
			perror("send");
			r = -1;
		}
	}
	return (r);
}

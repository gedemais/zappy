#include "zappy_client.h"


int			zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb)
{
	int r = 0;

	if (client->cmd_stack_size >= ZAPPY_CLIENT_MAX_STACKED_CMD) {
		r = ZAPPY_WAIT;
	}
	if (r == 0)
	{
		client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].cmd = cmd;
		client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].cb = cb;
		client->cmd_stack_size++;
		bzero(client->buf, 4096); // TODO
		fprintf(stderr, "%s: send {%s}\n----------\n", __func__, cmd);
		if (send(client->socket, cmd, len, 0) < 0) {
			perror("send");
			r = -1;
		}
	}
	return (r);
}

#include "zappy_client.h"


static int	broadcast_send(zappy_client_t *client)
{
	int		r = 0;
	uint8_t	len;
	char	buf[CLIENT_BUFSIZE] = {0};

	len = strlen((char *)client->player.broadcast_msg);
	// si on a un message a broadcast
	if (len > 0) {
		snprintf(buf, CLIENT_BUFSIZE, "%s %s", commands[CMD_BROADCAST].name, client->player.broadcast_msg);
		r = zappy_client_transceive(client, buf, CLIENT_BUFSIZE, zappy_broadcast_cb);
		WAIT_CALLBACK(client->task, r)
	} else { // sinon on reprend le loot
		client->task = PLAYER_TASK_LOOT;
	}
	// broadcast done donc on reset le type de broadcast a effectuer
	client->player.broadcast = BROADCAST_NONE;
	return (r);
}

static int	broadcast_inventaire(zappy_client_t *client)
{
	int	r = 0;

	// on prepare le broadcast pour que les autres joueurs actualisent leurs inventaires de team
	snprintf((char *)client->player.broadcast_msg, CLIENT_BUFSIZE,
		"%s %s,player_id %d,team_name %s",
		commands[CMD_INVENTAIRE].name, client->buf, client->player.id, client->team.name);
	return (r);
}

int			zappy_client_broadcast(zappy_client_t *client)
{
	int		r = 0;

	// si il n'y a pas de broadcast a effectuer alors return
	if (client->player.broadcast == BROADCAST_NONE)
		return (r);
	// on reset le broadcast_msg pour le formater celon le type de broadcast a effectuer
	bzero(client->player.broadcast_msg, CLIENT_BUFSIZE);
	if (client->player.broadcast == BROADCAST_INVENTAIRE) {
		r = broadcast_inventaire(client);
	}
	if (r == 0) {
		r = broadcast_send(client);
	}
	return (r);
}

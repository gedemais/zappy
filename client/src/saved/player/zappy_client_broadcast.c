#include "zappy_client.h"


static int	broadcast_send(zappy_client_t *client)
{
	int		r = 0;
	char	buf[CLIENT_BUFSIZE] = {0};

	// si on a un message a broadcast
	if (strlen((char *)client->player.broadcast_msg) > 0) {
		snprintf(buf, CLIENT_BUFSIZE, "%s %s", commands[CMD_BROADCAST].name, client->player.broadcast_msg);
		r = zappy_client_transceive(client, buf, CLIENT_BUFSIZE, zappy_broadcast_cb);
	}
	// broadcast done donc on reset le type de broadcast a effectuer
	client->player.broadcast = BROADCAST_NONE;
	// on reprend le loot
	client->task = PLAYER_TASK_LOOT;
	return (r);
}

static int	broadcast_inventaire(zappy_client_t *client)
{
	int	r = 0;

	// on prepare le broadcast pour que les autres joueurs actualisent leurs inventaires de team
	// format : inventaire:{...}-player_id:n-team_name:n
	snprintf((char *)client->player.broadcast_msg, CLIENT_BUFSIZE,
		"%s:%s-player_id:%d-team_name:%s",
		commands[CMD_INVENTAIRE].name, client->buf, client->player.id, client->team.name);
	return (r);
}

static int	broadcast_player(zappy_client_t *client)
{
	int	r = 0;

	// on prepare le broadcast pour demander à des joueurs de nous rejoindre
	// format : incantation-player_id:n-team_name:n
	snprintf((char *)client->player.broadcast_msg, CLIENT_BUFSIZE,
		"%s-player_id:%d-team_name:%s",
		commands[CMD_INCANTATION].name, client->player.id, client->team.name);
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
	if (client->player.broadcast == BROADCAST_PLAYER) {
		r = broadcast_player(client);
	}
	if (r == 0) {
		r = broadcast_send(client);
	}
	return (r);
}

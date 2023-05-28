#include "zappy_client.h"
#include "zappy_client_move.h"

# define FARMER_BUF_SIZE	4096
static char	buf[FARMER_BUF_SIZE] = {0};

int	zappy_player_loot_all(zappy_client_t *client)
{
	int r = 0;
	bool	found = false;

	for (int i = 0 ; !found && i < VISION_MAP_MAX ; i++) {
		for (int j = 0 ; !found && j < CASE_ELEMENTS ; j++) {
			if (client->player.vision_map[i * CASE_ELEMENTS + j] != 0)
			{
				found = true;

				r = zappy_client_move(client, i);
				if (r == 0) {
				 	snprintf(buf, FARMER_BUF_SIZE, "prend %s", ressources[j].name);
				 	r = zappy_client_transceive(client, buf, FARMER_BUF_SIZE, zappy_prend_cb);
				}
				if (r == 0)
				{
					client->player.vision_map[i * CASE_ELEMENTS + j]--;
				}
			}
		}
	}
	if (!found)
	{
		r = zappy_client_move_front(client);
		if (r == 0)
		{
			// A chaque "voir" get and broadcast inventory 
			client->task = PLAYER_TASK_GET_INVENTAIRE;
		}
	}
	return (r);
}

//int	zappy_player_broadcast_inventaire(zappy_client_t *client)
//{
//	int r = 0;
//	char buf[1024];
//
//	snprintf(buf, 1024,
//		"broadcast inventaire {"
//		"nourriture %d, "
//		"linemate %d, "
//		"deraumere %d, "
//		"sibur %d, "
//		"mendiane %d, "
//		"phiras %d, "
//		"thystame %d, "
//		"ttl %d"
//		"}, %d, %s",
//		client->player.inventaire[client->player.id].nourriture,
//		client->player.inventaire[client->player.id].linemate,
//		client->player.inventaire[client->player.id].deraumere,
//		client->player.inventaire[client->player.id].sibur,
//		client->player.inventaire[client->player.id].mendiane,
//		client->player.inventaire[client->player.id].phiras,
//		client->player.inventaire[client->player.id].thystame,
//		client->player.inventaire[client->player.id].ttl,
//		client->player.id,
//		client->team.name);
//	r = zappy_client_transceive(client, buf, strlen(buf), zappy_broadcast_cb);
//	return (r);
//}
//
/* the crazy player */
int 	zappy_player(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true;
	uint8_t len;

	client->task = PLAYER_TASK_ID;
	while (client->player.alive)
	{
		// reset du buffer :: pe inutile ?
		bzero(client->buf, CLIENT_BUFSIZE);
		r = zappy_client_receive(client);
		if (r == 0)
		{
			switch (client->task)
			{
				case (PLAYER_TASK_ID):
					// on recupere l'id du joueur et le nb de slot libre/max
					r = zappy_client_transceive(client, commands[CMD_CONNECT_NBR].name, commands[CMD_CONNECT_NBR].len, zappy_connect_nbr_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				case (PLAYER_TASK_GET_INVENTAIRE):
					// on recupere l'inventaire et le ttl
					r = zappy_client_transceive(client, commands[CMD_INVENTAIRE].name, commands[CMD_INVENTAIRE].len, zappy_inventaire_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				//case (PLAYER_TASK_BROADCAST_INVENTAIRE):
				//	// on recupere l'inventaire et le ttl
				//	r = zappy_player_broadcast_inventaire(client);
				//	break ;
				case (PLAYER_TASK_LOOK):
					// on recupere le champ de vision du joueur
					r = zappy_client_transceive(client, commands[CMD_VOIR].name, commands[CMD_VOIR].len, zappy_voir_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				case (PLAYER_TASK_LOOT):
					r = zappy_player_loot_all(client);
					// si on ramasse un loot on recupere l'inventaire pour le broadcast
					if (r == 0) {
						fprintf(stderr, "%s: move done n take send pos=%d\n", __func__, client->player.relative_pos);
					}
					break ;
				case (PLAYER_TASK_BROADCAST):
					len = strlen((char *)client->player.broadcast_msg);
					// si on a un message a broadcast
					if (len > 0) {
						snprintf(buf, CLIENT_BUFSIZE, "%s %s", commands[CMD_BROADCAST].name, client->player.broadcast_msg);
						r = zappy_client_transceive(client, buf, CLIENT_BUFSIZE, zappy_broadcast_cb);
						WAIT_CALLBACK(client->task, r)
					} else { // sinon on reprend le loot
						client->task = PLAYER_TASK_LOOT;
					}
					break ;
				default:
					break ;
			}
		}
		if (r == -1) {
			client->player.alive = false;
		}
	}
	return (r);
}

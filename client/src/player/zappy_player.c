#include "zappy_client.h"


/* the crazy player */
int 	zappy_player(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true, found;
	char	buf[CLIENT_BUFSIZE] = {0};

	client->task = PLAYER_TASK_ID;
	while (client->player.alive)
	{
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
				case (PLAYER_TASK_LOOK):
					// on recupere le champ de vision du joueur
					r = zappy_client_transceive(client, commands[CMD_VOIR].name, commands[CMD_VOIR].len, zappy_voir_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				case (PLAYER_TASK_LOOT):
					r = zappy_client_loot(client);
					break ;
				case (PLAYER_TASK_BROADCAST):
					r = zappy_client_broadcast(client);
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

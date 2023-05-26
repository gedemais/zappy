#include "zappy_client.h"
#include "zappy_client_move.h"

# define PLAYER_BUF_SIZE	256


/* the crazy farmer */
int 	zappy_player(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true;
	bool	found;
	char	buf[PLAYER_BUF_SIZE] = {0};

	client->task = PLAYER_TASK_ID;
	while (run)
	{
		r = zappy_client_receipt(client);
		if (r == 0)
		{
			switch (client->task)
			{
				case (PLAYER_TASK_ID):
					// il faut que le joueur recupere son id en priorite
					r = zappy_client_transceive(client, commands[CMD_CONNECT_NBR].name, commands[CMD_CONNECT_NBR].len, zappy_connect_nbr_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				case (PLAYER_TASK_LOOK):
					r = zappy_client_transceive(client, commands[CMD_VOIR].name, commands[CMD_VOIR].len, zappy_voir_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				case (PLAYER_TASK_LOOT):
					found = false;
					for (int i = 0 ; !found && i < VISION_MAP_MAX ; i++) {
						for (int j = 0 ; !found && j < CASE_ELEMENTS ; j++) {
							if (client->player.vision_map[i * CASE_ELEMENTS + j] != 0)
							{
								found = true;

								// r = zappy_client_move(client, i);
								// if (r == 0) {
								// 	client->player.vision_map[i * CASE_ELEMENTS + j]--;
								// 	fprintf(stderr, "%s: move done pos=%d resources={%s}\n", __func__, client->player.relative_pos, case_ressources[j]);
								// }

								// if (r == 0) {
								// 	snprintf(buf, PLAYER_BUF_SIZE, "prend %s\n", case_ressources[j]);
								// 	r = zappy_client_transceive(client, buf, PLAYER_BUF_SIZE, zappy_prend_cb);
								// }

								// si on ramasse un loot recupere l'inventaire pour le broadcast
								if (r == 0) {
									r = zappy_client_transceive(client, commands[CMD_INVENTAIRE].name, commands[CMD_INVENTAIRE].len, zappy_inventaire_cb);
								}
								WAIT_CALLBACK(client->task, r)
							}
						}
					}
					if (!found) {
						client->task = PLAYER_TASK_LOOK;
					}
					break ;
				case (PLAYER_TASK_BROADCAST):
					// on a recupere l'inventaire donc on le broadcast
					snprintf(buf, PLAYER_BUF_SIZE, "%s %s\n", commands[CMD_BROADCAST].name, client->player.inventaire);
					r = zappy_client_transceive(client, buf, PLAYER_BUF_SIZE, zappy_broadcast_cb);
					WAIT_CALLBACK(client->task, r)
					break ;
				default:
					break ;
			}
		}
		if (r == -1) {
			run = false;
		}
	}
	return (r);
}

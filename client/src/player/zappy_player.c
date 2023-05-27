#include "zappy_client.h"
#include "zappy_client_move.h"


/* the crazy farmer */
int 	zappy_player(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true, found;
	char	buf[CLIENT_BUFSIZE] = {0};
	uint8_t len;

	client->task = PLAYER_TASK_ID;
	while (run)
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
				case (PLAYER_TASK_LOOK):
					// on recupere le champ de vision du joueur
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
								
								// =========================
								// WIP :: TASK LOOT a revoir
								// =========================

								// r = zappy_client_move(client, i);
								// if (r == 0) {
								// 	client->player.vision_map[i * CASE_ELEMENTS + j]--;
								// 	fprintf(stderr, "%s: move done pos=%d resources={%s}\n", __func__, client->player.relative_pos, case_ressources[j]);
								// }

								// if (r == 0) {
								// 	snprintf(buf, PLAYER_BUF_SIZE, "prend %s\n", case_ressources[j]);
								// 	r = zappy_client_transceive(client, buf, PLAYER_BUF_SIZE, zappy_prend_cb);
								// }

								// si on ramasse un loot on recupere l'inventaire pour le broadcast
								if (r == 0) {
									r = zappy_client_transceive(client, commands[CMD_INVENTAIRE].name, commands[CMD_INVENTAIRE].len, zappy_inventaire_cb);
								}
							}
						}
					}
					if (!found) {
						client->task = PLAYER_TASK_LOOK;
					}
					break ;
				case (PLAYER_TASK_BROADCAST):
					len = strlen((char *)client->player.broadcast_msg);
					// si on a un message a broadcast
					if (len > 0) {
						snprintf(buf, CLIENT_BUFSIZE, "%s %s\n", commands[CMD_BROADCAST].name, client->player.broadcast_msg);
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
			run = false;
		}
	}
	return (r);
}

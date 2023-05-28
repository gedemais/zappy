#include "zappy_client.h"


int			zappy_client_loot(zappy_client_t *client)
{
	int		r = 0;
	bool	found = false;

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
					WAIT_CALLBACK(client->task, r)
				}
			}
		}
	}
	if (!found) {
		client->task = PLAYER_TASK_LOOK;
	}
	return (r);
}
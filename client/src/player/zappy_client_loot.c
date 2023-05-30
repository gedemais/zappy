#include "zappy_client.h"


int			zappy_client_loot(zappy_client_t *client)
{
	int		r = 0;
	bool	found = false;
	char	buf[CLIENT_BUFSIZE] = {0};

	for (int i = 0 ; !found && i < VISION_MAP_MAX ; i++) {
		for (int j = 0 ; !found && j < CASE_ELEMENTS ; j++) {
			if (client->player.vision_map[i * CASE_ELEMENTS + j] != 0)
			{
				found = true;

				r = zappy_client_move(client, i);
				if (r == 0) {
				 	snprintf(buf, CLIENT_BUFSIZE, "prend %s", ressources[j].name);
				 	r = zappy_client_transceive(client, buf, CLIENT_BUFSIZE, zappy_prend_cb);
				}
				if (r == 0) {
					client->player.vision_map[i * CASE_ELEMENTS + j]--;
				}
			}
		}
	}
	if (!found)
	{
		r = zappy_client_move_front(client);
		if (r == 0) {
			// A chaque "voir" get and broadcast inventory
			client->task = PLAYER_TASK_GET_INVENTAIRE;
		}
	}
	if (r == 0) {
		fprintf(stderr, "%s: move done n take send pos=%d\n", __func__, client->player.relative_pos);
	}
	return (r);
}

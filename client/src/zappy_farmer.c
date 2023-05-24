#include "zappy_client.h"
#include "zappy_client_move.h"


int		zappy_client_broadcast_inventory_cb(zappy_client_t *client)
{
	int		r = 0;

	client->task = ZAPPY_FARMER_BROADCAST;
	return (r);
}

/* the crazy farmer */
int 	zappy_farmer(zappy_client_t *client)
{
	int r = 0;
	bool run = true;

	client->task = ZAPPY_FARMER_LOOK;
	while (run)
	{
		r = zappy_client_receipt(client);
		if (r == 0)
		{
			switch (client->task)
			{
				case (ZAPPY_FARMER_LOOK): {
					r = zappy_client_transceive(client, see_cmd, strlen(see_cmd), zappy_client_see_cb);
					if (r == 0)
					{
						client->task = ZAPPY_FARMER_LOOKWAIT;
					}
					break;
			  	}
				case (ZAPPY_FARMER_LOOT): {
					bool found = false;
					for (int i = 0 ; !found && i < VISION_MAP_MAX ; i++) {
						for (int j = 0 ; !found && j < CASE_ELEMENTS ; j++) {
							if (client->vision_map[i * CASE_ELEMENTS + j] != 0)
							{
								found = true;

								// r = zappy_client_move(client, i);
								// if (r == 0) {
								// 	client->vision_map[i * CASE_ELEMENTS + j]--;
								// 	fprintf(stderr, "%s: move done pos=%d resources={%s}\n", __func__, client->relative_pos, case_ressources[j]);
								// }

								// if (r == 0) {
								// 	char buf[64] = {0};
								// 	snprintf(buf, 64, "prend %s",
								// 			case_ressources[j]);
								// 	r = zappy_client_transceive(client, buf, strlen(buf), NULL);
								// }

								// si on ramasse un loot recupere l'inventaire pour le broadcast
								if (r == 0) {
									r = zappy_client_transceive(client, "inventaire", strlen("inventaire"), zappy_client_broadcast_inventory_cb);
								}
							}
						}
					}
					if (!found)
					{
						client->task = ZAPPY_FARMER_LOOK;
					}
			  	}
				case (ZAPPY_FARMER_BROADCAST): {
					char	buf[256] = {0};
	
					// on a recupere l'inventaire donc on le broadcast
					snprintf(buf, 256, "broadcast %s", client->buf);
					r = zappy_client_transceive(client, buf, strlen(buf), NULL);
					if (r == 0) {
						// une fois broadcast on reprend le loot ou le look
						client->task = ZAPPY_FARMER_LOOT;
					}
				}
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

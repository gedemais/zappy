#include "zappy_client.h"
#include "zappy_client_move.h"


int		zappy_client_broadcast_inventory_cb(zappy_client_t *client)
{
	int	r = 0;

	(void)client;
	if (r == 0)
	{
		fprintf(stderr, "broadcasting inventory callback\n");
	}
	return (r);
}

int		zappy_farmer_broadcast(zappy_client_t *client, int option)
{
	fprintf(stderr, "%s\n", __func__);
	int	r = 0;

	switch (option)
	{
		case (BROADCAST_INVENTORY_SEND): {
			r = zappy_client_transceive(client, broadcast_cmd, strlen(broadcast_cmd), zappy_client_broadcast_inventory_cb);

			printf("client transceive - r: %d\n", r);
			if (r == 0) {
				fprintf(stderr, "%s\n", client->buf);
			}
			break ;
		}
		default:
			break ;
	}
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

								r = zappy_client_move(client, i);
								if (r == 0) {
									client->vision_map[i * CASE_ELEMENTS + j]--;
									fprintf(stderr, "%s: move done pos=%d resources={%s}\n", __func__, client->relative_pos, case_ressources[j]);
								}

								// if (r == 0) {
								// 	char buf[64] = {0};
								// 	snprintf(buf, 64, "prend %s",
								// 			case_ressources[j]);
								// 	r = zappy_client_transceive(client, buf, strlen(buf), NULL);
								// }

								// si on ramasse un loot on broadcast linventaire
								client->task = ZAPPY_FARMER_BROADCAST;
							}
						}
					}
					if (!found)
					{
						client->task = ZAPPY_FARMER_LOOK;
					}
			  	}
				case (ZAPPY_FARMER_BROADCAST): {
					zappy_farmer_broadcast(client, BROADCAST_INVENTORY_SEND);
					client->task = ZAPPY_FARMER_LOOT;
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

#include "zappy_client.h"
#include "zappy_client_move.h"

# define FARMER_BUF_SIZE	256


/* the crazy farmer */
int 	zappy_farmer(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true;
	bool	found;
	char	buf[FARMER_BUF_SIZE] = {0};

	client->task = ZAPPY_FARMER_LOOK;
	while (run)
	{
		r = zappy_client_receipt(client);
		if (r == 0)
		{
			switch (client->task)
			{
				case (ZAPPY_FARMER_LOOK):
					r = zappy_client_transceive(client, see_cmd, strlen(see_cmd), zappy_voir_cb);
					if (r == 0)
					{
						client->task = ZAPPY_FARMER_LOOKWAIT;
					}
					break ;
				case (ZAPPY_FARMER_LOOT):
					found = false;
					for (int i = 0 ; !found && i < VISION_MAP_MAX ; i++) {
						for (int j = 0 ; !found && j < CASE_ELEMENTS ; j++) {
							if (client->vision_map[i * CASE_ELEMENTS + j] != 0)
							{
								found = true;

								// fprintf(stderr, "farmer is on { %s }\n", case_ressources[j]);

								// r = zappy_client_move(client, i);
								// if (r == 0) {
								// 	client->vision_map[i * CASE_ELEMENTS + j]--;
								// 	fprintf(stderr, "%s: move done pos=%d resources={%s}\n", __func__, client->relative_pos, case_ressources[j]);
								// }

								// if (r == 0) {
								// 	bzero(buf, FARMER_BUF_SIZE);
								// 	strcat(buf, "prend ");
								// 	strcat(buf, case_ressources[j]);
								// 	r = zappy_client_transceive(client, buf, FARMER_BUF_SIZE, zappy_prend_cb);
								// }

								// si on ramasse un loot recupere l'inventaire pour le broadcast
								if (r == 0) {
									r = zappy_client_transceive(client, "inventaire", strlen("inventaire"), zappy_inventaire_cb);
								}
							}
						}
					}
					if (!found)
					{
						client->task = ZAPPY_FARMER_LOOK;
					}
					break ;
				case (ZAPPY_FARMER_BROADCAST):
					// on a recupere l'inventaire donc on le broadcast
					if (r == 0) {
						bzero(buf, FARMER_BUF_SIZE);
						strcat(buf, "broadcast ");
						strcat(buf, client->inventory);
						r = zappy_client_transceive(client, buf, FARMER_BUF_SIZE, zappy_broadcast_cb);
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

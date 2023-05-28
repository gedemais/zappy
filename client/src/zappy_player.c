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
				 	snprintf(buf, FARMER_BUF_SIZE, "prend %s", case_ressources[j]);
				 	r = zappy_client_transceive(client, buf, FARMER_BUF_SIZE, zappy_prend_cb);
				}
				if (r == 0)
				{
					client->player.vision_map[i * CASE_ELEMENTS + j]--;
				}
				if (r == 0) {
					// si on ramasse un loot recupere l'inventaire pour le broadcast
					r = zappy_client_transceive(client, "inventaire", strlen("inventaire"), zappy_broadcast_cb);
				}
			}
		}
	}
	if (!found)
	{
		r = zappy_client_move_front(client);
		if (r == 0)
		{
			client->task = ZAPPY_FARMER_LOOK;
		}
	}
	return (r);
}

/* the crazy player */
int 	zappy_player(zappy_client_t *client)
{
	int		r = 0;
	bool	run = true;

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
					r = zappy_player_loot_all(client);
					if (r == 0)
					{
						fprintf(stderr, "%s: move done n take send pos=%d\n", __func__, client->player.relative_pos);
					}
					break ;
				case (ZAPPY_FARMER_BROADCAST):
					// on a recupere l'inventaire donc on le broadcast
					if (r == 0) {
						bzero(buf, FARMER_BUF_SIZE);
						strcat(buf, "broadcast ");
						strcat(buf, client->player.inventaire);
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

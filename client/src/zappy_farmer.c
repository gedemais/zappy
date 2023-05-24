#include "zappy_client.h"
#include "zappy_client_move.h"

#define ZAPPY_FARMER_LOOK 1
#define ZAPPY_FARMER_LOOT 2

/* the crazy farmer */
int zappy_farmer(zappy_client_t *client)
{
	int r = 0;
	bool run = true;
	int task = ZAPPY_FARMER_LOOK;

	while (run)
	{
		switch (task)
		{
			case (ZAPPY_FARMER_LOOK): {
				r = zappy_client_transceive(client, see_cmd, strlen(see_cmd), NULL);
				if (r == 0) {
					r = zappy_client_parse_see(client);
				}
				if (r == 0) {
					task = ZAPPY_FARMER_LOOT;
				}
				zappy_debug_print_vision_map(client);
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
							// 	r = zappy_client_transceive(client, buf, strlen(buf), zappy_rsp_ok);
							// }
						}
					}
				}
				if (!found)
				{
					task = ZAPPY_FARMER_LOOK;
				}
		  	}
			default:
				break ;
		}
		if (r != 0) {
			run = false;
		}
	}
	return (r);
}

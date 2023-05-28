#include "zappy_client.h"


static int	zappy_client_parse_voir(zappy_client_t *client)
{
	int r = 0;
	int i = 0;
	int c = 0;

	bzero(client->player.vision_map, VISION_MAP_MAX * CASE_ELEMENTS);
	client->player.relative_pos = 0;
	client->player.relative_orientation = 0;
	if (client->buf[i++] != '{') {
		r = -1;
	}
	while (r == 0 && i < (int)strlen((char*)client->buf))
	{
		if (client->buf[i] == '}') {
			break ;
		}
		else if (client->buf[i] == ',') {
			c++;
		}
		else if (client->buf[i] != ' ')
		{
			bool b = false;
			for (int j = 0; j < R_MAX; j++) {
				if (ressources[j].name == NULL)
					break ;
				if (!memcmp(ressources[j].name, (char*)&client->buf[i], ressources[j].len)) {
					b = true;
					client->player.vision_map[c * CASE_ELEMENTS + j]++;
					i += ressources[j].len;
				}
			}
			if (b == false) {
				r = -1;
			}
		}
		i++;
	}
	return (r);
}

static void	zappy_debug_print_vision_map(zappy_client_t *client)
{
	(void)client;
	return ;

	for (int i = 0 ; i < VISION_MAP_MAX ; i++) {
		fprintf(stderr, "CASE[%3d] : L:%d D:%d S:%d M:%d P:%d T:%d F:%d P:%d\n", i,
				client->player.vision_map[i * CASE_ELEMENTS],
				client->player.vision_map[i * CASE_ELEMENTS + 1],
				client->player.vision_map[i * CASE_ELEMENTS + 2],
				client->player.vision_map[i * CASE_ELEMENTS + 3],
				client->player.vision_map[i * CASE_ELEMENTS + 4],
				client->player.vision_map[i * CASE_ELEMENTS + 5],
				client->player.vision_map[i * CASE_ELEMENTS + 6],
				client->player.vision_map[i * CASE_ELEMENTS + 7]);
	}
}

int			zappy_voir_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	int r = 0;
	(void)cmd;

	r = zappy_client_parse_voir(client);
	if (r == 0)
	{
		zappy_debug_print_vision_map(client);
		client->player.relative_pos = 0;
		client->player.relative_orientation = 0;
		client->task = PLAYER_TASK_LOOT;
	}
	return (r);
}

#include "zappy_client.h"


static int	zappy_client_parse_voir(zappy_client_t *client)
{
	int r = 0;
	int i = 0;
	int c = 0;

	fprintf(stderr, "%s\n buf: %s\n", __func__, client->buf);

	bzero(client->player.vision_map, VISION_MAP_MAX * CASE_ELEMENTS);
	client->player.relative_pos = 0;
	client->player.relative_orientation = 0;
	if (client->buf[i++] != '{') {
		r = -1;
	}
	fprintf(stderr, "before while\n");
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
			for (int j = 0 ; case_ressources[j] ; j++) {
				if (!memcmp(case_ressources[j],
							(char*)&client->buf[i], strlen(case_ressources[j]))) {
					b = true;
					client->player.vision_map[c * CASE_ELEMENTS + j]++;
					i += (int)strlen(case_ressources[j]);
				}
			}
			if (b == false)
			{
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

int			zappy_voir_cb(zappy_client_t *client)
{
	int r = 0;

	fprintf(stderr, "%s\n", __func__);
	r = zappy_client_parse_voir(client);
	if (r == 0)
	{
		fprintf(stderr, "r == 0\n");
		client->task = PLAYER_TASK_LOOT;
		zappy_debug_print_vision_map(client);
		client->player.relative_pos = 0;
		client->player.relative_orientation = 0;
	}
	return (r);
}

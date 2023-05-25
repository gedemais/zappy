#include "zappy_client.h"


int			zappy_client_move_cb(zappy_client_t *client)
{
	int r = 0;

	if (memcmp(client->buf, "ok", strlen("ok")))
	{
		r = -1;
	}
	return (r);
}

// ============================================================================

int		zappy_avance_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_droite_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_gauche_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

static int		zappy_client_parse_see(zappy_client_t *client)
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


int		zappy_voir_cb(zappy_client_t *client)
{
	int r = 0;

	r = zappy_client_parse_see(client);
	if (r == 0)
	{
		client->task = ZAPPY_FARMER_LOOT;
		zappy_debug_print_vision_map(client);
	}
	return (r);
}

// ============================================================================

int		zappy_inventaire_cb(zappy_client_t *client)
{
	int	r = 0;

	client->task = ZAPPY_FARMER_BROADCAST;
	memcpy(client->player.inventaire, client->buf, CLIENT_BUFSIZE);
	return (r);
}

// ============================================================================

int		zappy_prend_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_pose_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_expulse_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_broadcast_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	client->task = ZAPPY_FARMER_LOOT;
	return (r);
}

// ============================================================================

int		zappy_incantation_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_fork_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

int		zappy_connect_nbr_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

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
		client->task = PLAYER_TASK_LOOT;
		zappy_debug_print_vision_map(client);
		client->player.relative_pos = 0;
		client->player.relative_orientation = 0;
	}
	return (r);
}

// ============================================================================

// {9 nourriture, 0 linemate, 0 deraumere, 0 sibur, 0 mendiane, 0 phiras, 0 thystame}
t_inventaire	deserialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE])
{
	(void)inventaire_str;

	t_inventaire	inventaire;

	memset(&inventaire, 0, sizeof(t_inventaire));
	return (inventaire);
}

char			*serialize_inventaire(t_inventaire inventaire)
{
	(void)inventaire;

	char	*inventaire_str = NULL;

	return (inventaire_str);
}

int				zappy_inventaire_cb(zappy_client_t *client)
{
	int	r = 0;

	// on stock linventaire du joueur
	client->player.inventaire[client->player.id] = deserialize_inventaire(client->buf);
	// on ce prepare a le broadcast aux autres joueurs pour qu'ils actualisent leurs inventaires de team
	memcpy(client->player.broadcast_msg, client->buf, CLIENT_BUFSIZE);
	client->task = PLAYER_TASK_BROADCAST;
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
	int		r = 0;

	// le broadcast a ete effectue donc on reset le buffer du message
	bzero(client->player.broadcast_msg, CLIENT_BUFSIZE);
	// et on remet le joueur au travail
	client->task = PLAYER_TASK_LOOT;
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
	int		r = 0;

	// TODO :: si le nb de joueur dans la team n'est pas au max il faudra hatch
	// on donne au joueur un id
	if (client->player.id == 0) {
		client->player.id = 1; // default is 1 : waiting for api update
	}
	// on ce prepare a le mettre au travail
	client->task = PLAYER_TASK_LOOK;
	return (r);
}

// ============================================================================

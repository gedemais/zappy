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

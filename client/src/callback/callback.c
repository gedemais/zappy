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

// response: ok/ko
int		zappy_avance_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_droite_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_gauche_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_prend_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_pose_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_expulse_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
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

// response: elevation en cours
// niveau actuel : K
int		zappy_incantation_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_fork_cb(zappy_client_t *client)
{
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

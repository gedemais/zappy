#include "zappy_client.h"


int			zappy_client_move_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	int r = 0;

	if (memcmp(client->buf, "ok", strlen("ok")))
	{
		r = -1;
	}
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_avance_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)client;
	(void)cmd;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_droite_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)client;
	(void)cmd;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_gauche_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)client;
	(void)cmd;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_prend_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)client;
	(void)cmd;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_pose_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_expulse_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_broadcast_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	int		r = 0;

	client->task = PLAYER_TASK_LOOK;
	return (r);
}

// ============================================================================

// response: elevation en cours
// niveau actuel : K
int		zappy_incantation_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_fork_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

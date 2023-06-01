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
	(void)cmd;

	int		r = 0;
	char	*str = (char *)client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].str;

	fprintf(stderr, "%s: buf: %s\n", __func__, str);
	// on recupere une ressource donc on update l'inventaire du joueur
	// format : "prend ressource"
	// on avance le ptr de "prend "
	update_inventaire(&client->player.inventaire[client->player.id],
		str + commands[CMD_PREND].len + 1, true);
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

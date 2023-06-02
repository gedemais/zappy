#include "zappy_client.h"


int			zappy_client_move_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;

	int r = 0;

	if (memcmp(client->buf, "ok", strlen("ok"))) {
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

	int				r = 0;
	char			*cmd_buf = (char *)client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].buf;
	t_inventaire	inventaire = client->player.inventaire[client->player.id];

	// on recupere une ressource donc on update l'inventaire du joueur
	// format : "prend ressource" => on avance le ptr de strlen("prend ")
	update_inventaire(&inventaire, cmd_buf + commands[CMD_PREND].len + 1, true);
	print_inventaire(inventaire);
	// on verifie si le joueur à les ressources pour lvl up
	// c'est la fonction zappy_client_incantation qui va s'en charger
	client->task = PLAYER_TASK_INCANTATION;
	return (r);
}

// ============================================================================

// response: ok/ko
int		zappy_pose_cb(zappy_client_t *client, zappy_client_cmd_t *cmd)
{
	(void)cmd;

	int				r = 0;
	char			*cmd_buf = (char *)client->cmds[(client->cmd_idx + client->cmd_stack_size) % ZAPPY_CLIENT_MAX_STACKED_CMD].buf;
	t_inventaire	inventaire = client->player.inventaire[client->player.id];

	// on pose une ressource donc on update l'inventaire du joueur
	// format : "pose ressource" => on avance le ptr de strlen("pose ")
	update_inventaire(&inventaire, cmd_buf + commands[CMD_POSE].len + 1, false);
	print_inventaire(inventaire);
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
	(void)client;

	int		r = 0;

	// fprintf(stderr, "%s\n", __func__);
	return (r);
}

// ============================================================================

// response: elevation en cours/ko
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

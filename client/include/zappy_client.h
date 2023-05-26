#ifndef ZAPPY_CLIENT_H
# define ZAPPY_CLIENT_H


# include <stdint.h>
# include <arpa/inet.h>
# include <stdio.h>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdbool.h>

# include "zappy_client_getopt.h"

# define CLIENT_BUFSIZE 4096

# define ZAPPY_CLIENT_MAX_STACKED_CMD 1

# define ZAPPY_OK	0
# define ZAPPY_WAIT	1
# define ZAPPY_ERROR -1

# define VISION_MAP_MAX		81
# define CASE_ELEMENTS		8

# define ORIENTATION_FRONT		0
# define ORIENTATION_RIGHT		1
# define ORIENTATION_BACK		2
# define ORIENTATION_LEFT		3


enum			e_commands
{
	CMD_AVANCE,
	CMD_DROITE,
	CMD_GAUCHE,
	CMD_VOIR,
	CMD_INVENTAIRE,
	CMD_PREND,
	CMD_POSE,
	CMD_EXPULSE,
	CMD_BROADCAST,
	CMD_INCANTATION,
	CMD_FORK,
	CMD_CONNECT_NBR,
	CMD_MAX
};

typedef struct	s_zappy_cmds
{
	char	name[256];
	uint8_t	len;
}				t_cmds;

static t_cmds	commands[CMD_MAX] = {
	[CMD_AVANCE]		= {.name = "avance", .len = strlen("avance")},
	[CMD_DROITE]		= {.name = "droite", .len = strlen("droite")},
	[CMD_GAUCHE]		= {.name = "gauche", .len = strlen("gauche")},
	[CMD_VOIR]			= {.name = "voir", .len = strlen("voir")},
	[CMD_INVENTAIRE]	= {.name = "inventaire", .len = strlen("inventaire")},
	[CMD_PREND]			= {.name = "prend", .len = strlen("prend")},
	[CMD_POSE]			= {.name = "pose", .len = strlen("pose")},
	[CMD_EXPULSE]		= {.name = "expulse", .len = strlen("expulse")},
	[CMD_BROADCAST]		= {.name = "broadcast", .len = strlen("broadcast")},
	[CMD_INCANTATION]	= {.name = "incantation", .len = strlen("incantation")},
	[CMD_FORK]			= {.name = "fork", .len = strlen("fork")},
	[CMD_CONNECT_NBR]	= {.name = "connect_nbr", .len = strlen("connect_nbr")}
};

static char *zappy_rsp_ok = "ok";

/* Some helpfull static const for relative vision_map array */
static int vision_row_size[] = {
		1,
		3,
		5,
		7,
		9,
		11,
		13,
		15,
		17
};
static int vision_row_start[] = {
		0,
		1,
		4,
		9,
		16,
		25,
		36,
		49,
		64
};
static int vision_row_center[] = {
		0,
		2,
		6,
		12,
		20,
		30,
		42,
		56,
		72
};

enum	e_ressources
{
	R_LINEMATE,
	R_DERAUMERE,
	R_SIBUR,
	R_MENDIANE,
	R_PHIRAS,
	R_THYSTAME,
	R_NOURRITURE,
	R_PLAYER,
	R_MAX
};

static char *case_ressources[R_MAX] =
{
	[R_LINEMATE]	= "linemate",
	[R_DERAUMERE]	= "deraumere",
	[R_SIBUR]		= "sibur",
	[R_MENDIANE]	= "mendiane",
	[R_PHIRAS]		= "phiras",
	[R_THYSTAME]	= "thystame",
	[R_NOURRITURE]	= "nourriture",
	[R_PLAYER]		= "player"
};

typedef struct zappy_client_s zappy_client_t;

typedef int (*zappy_client_cmd_cb_t)(zappy_client_t *);

typedef struct zappy_client_cmd_s
{
	char					*cmd;
	uint8_t					id, lvl, option;
	zappy_client_cmd_cb_t	cb;
} zappy_client_cmd_t;

enum	e_player_task {
	PLAYER_TASK_WAIT,
	PLAYER_TASK_ID,
	PLAYER_TASK_LOOK,
	PLAYER_TASK_LOOT,
	PLAYER_TASK_BROADCAST,
	PLAYER_TASK_MAX
};

typedef struct zappy_player_s
{
	uint8_t		id, lvl;
	int			pos_x; // Absolute position, unused ?
	int			pos_y; // Absolute position, unused ?
	uint8_t		vision_map[VISION_MAP_MAX * CASE_ELEMENTS];
	uint8_t		relative_pos; // Relative position for vision map
	uint8_t		relative_orientation; // Relative orientation, always start at 0 when see
									  // The absolute orientation is not known of client
									  // At each "see" the orientation is back to 0
	char		inventaire[CLIENT_BUFSIZE];
}				t_player;

typedef struct zappy_client_s
{
	int					socket;
	struct sockaddr_in	sockaddr;
	uint8_t				buf[CLIENT_BUFSIZE];

	t_player			player;

	zappy_client_cmd_t	cmds[ZAPPY_CLIENT_MAX_STACKED_CMD];
	uint8_t				cmd_idx; // idx used to rotate cmds
	uint8_t				cmd_stack_size; // nb of elements currently in cmds
	uint8_t				task;
}				zappy_client_t;

/* main function called from main*/
int		zappy_client(zappy_client_opt_t *opt);
/* transceive blocking function TODO : client should use select(2) for handle "mort" and broadcast msg */
int		zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb);

int		zappy_client_see_cb(zappy_client_t *client);
int		zappy_client_receipt(zappy_client_t *client);

// callback
int		zappy_client_move_cb(zappy_client_t *client);

int		zappy_avance_cb(zappy_client_t *client);
int		zappy_droite_cb(zappy_client_t *client);
int		zappy_gauche_cb(zappy_client_t *client);
int		zappy_voir_cb(zappy_client_t *client);
int		zappy_inventaire_cb(zappy_client_t *client);
int		zappy_prend_cb(zappy_client_t *client);
int		zappy_pose_cb(zappy_client_t *client);
int		zappy_expulse_cb(zappy_client_t *client);
int		zappy_broadcast_cb(zappy_client_t *client);
int		zappy_incantation_cb(zappy_client_t *client);
int		zappy_fork_cb(zappy_client_t *client);
int		zappy_connect_nbr_cb(zappy_client_t *client);


#endif

#ifndef ZAPPY_CLIENT_H
# define ZAPPY_CLIENT_H


# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <strings.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/socket.h>

# include "libft.h"

# include "zappy_client_getopt.h"
# include "zappy_client_move.h"


# define CLIENT_BUFSIZE 4096
# define PLAYER_MAX		6

# define ZAPPY_CLIENT_MAX_STACKED_CMD 1

# define ZAPPY_OK		0
# define ZAPPY_WAIT		1
# define ZAPPY_ERROR	-1

# define VISION_MAP_MAX		81
# define CASE_ELEMENTS		8

# define ORIENTATION_FRONT	0
# define ORIENTATION_RIGHT	1
# define ORIENTATION_BACK	2
# define ORIENTATION_LEFT	3

# define WAIT_CALLBACK(task, r)		if (r == 0) { task = PLAYER_TASK_WAIT; }


static char	*zappy_rsp_ok = "ok";

/* Some helpfull static const for relative vision_map array */
static int	vision_row_size[] = {
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
static int	vision_row_start[] = {
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

static int	vision_row_center[] = {
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

typedef struct	s_zappy_ressources
{
	char	name[256];
	uint8_t	len;
}				t_loot;

enum			e_ressources
{
	R_NOURRITURE,
	R_LINEMATE,
	R_DERAUMERE,
	R_SIBUR,
	R_MENDIANE,
	R_PHIRAS,
	R_THYSTAME,
	R_PLAYER,
	R_MAX
};

static t_loot	ressources[R_MAX] = {
	[R_LINEMATE]	= {.name = "linemate", .len = strlen("linemate")},
	[R_DERAUMERE]	= {.name = "deraumere", .len = strlen("deraumere")},
	[R_SIBUR]		= {.name = "sibur", .len = strlen("sibur")},
	[R_MENDIANE]	= {.name = "mendiane", .len = strlen("mendiane")},
	[R_PHIRAS]		= {.name = "phiras", .len = strlen("phiras")},
	[R_THYSTAME]	= {.name = "thystame", .len = strlen("thystame")},
	[R_NOURRITURE]	= {.name = "nourriture", .len = strlen("nourriture")},
	[R_PLAYER]		= {.name = "player", .len = strlen("player")}
};

typedef struct	zappy_client_s zappy_client_t;

typedef int		(*zappy_client_cmd_cb_t)(zappy_client_t *);

typedef struct	zappy_client_cmd_s
{
	char					*cmd;
	uint8_t					id, lvl, option;
	zappy_client_cmd_cb_t	cb;
}				zappy_client_cmd_t;

enum	e_player_task {
	PLAYER_TASK_WAIT,
	PLAYER_TASK_ID,
	PLAYER_TASK_LOOK,
	PLAYER_TASK_LOOT,
	PLAYER_TASK_BROADCAST,
	PLAYER_TASK_MAX
};

enum			e_broadcast
{
	BROADCAST_NONE,
	BROADCAST_INVENTAIRE,
	BROADCAST_MAX
};

typedef struct	s_zappy_inventaire
{
	int		nourriture;
	int		linemate;
	int		deraumere;
	int		sibur;
	int		mendiane;
	int		phiras;
	int		thystame;
	int		ttl;
}				t_inventaire;

typedef struct	s_zappy_player
{
	uint8_t			id, lvl;
	int				pos_x; // Absolute position, unused ?
	int				pos_y; // Absolute position, unused ?
	uint8_t			vision_map[VISION_MAP_MAX * CASE_ELEMENTS];
	uint8_t			relative_pos; // Relative position for vision map
	uint8_t			relative_orientation; // Relative orientation, always start at 0 when see
									  // The absolute orientation is not known of client
									  // At each "see" the orientation is back to 0
	t_inventaire	inventaire[PLAYER_MAX];
	uint8_t			broadcast;	// permet d'identifier le type de broadcast a effectuer
	uint8_t			broadcast_msg[CLIENT_BUFSIZE]; // stock le broadcast_msg
}				t_player;

typedef struct	s_zappy_team
{
	uint8_t			size; // team max size
	uint8_t			nb_player; // current nb of players
	char			*name;
	t_inventaire	inventaire;
}				t_team;

typedef struct	zappy_client_s
{
	int					socket;
	struct sockaddr_in	sockaddr;
	uint8_t				buf[CLIENT_BUFSIZE];

	t_player			player;
	t_team				team;

	zappy_client_cmd_t	cmds[ZAPPY_CLIENT_MAX_STACKED_CMD];
	uint8_t				cmd_idx; // idx used to rotate cmds
	uint8_t				cmd_stack_size; // nb of elements currently in cmds
	uint8_t				task;
}				zappy_client_t;

/* argvs parsing */
int		zappy_client_getopt(int ac, char **av, zappy_client_opt_t *opt);
/* main function called from main*/
int		zappy_client(zappy_client_opt_t *opt);
/* transceive blocking function TODO : client should use select(2) for handle "mort" and broadcast msg */
int		zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb);
int		zappy_client_receive(zappy_client_t *client);

/* PLAYER'S TASK FUNCTIONS */
// Zappy player, the happy-go-lucky farmer
int		zappy_player(zappy_client_t *client);
int		zappy_client_broadcast(zappy_client_t *client);
int		zappy_client_loot(zappy_client_t *client);

// server's messages handlers
int		zappy_message(zappy_client_t *client);
int		zappy_deplacement(zappy_client_t *client);
int		zappy_mort(zappy_client_t *client);

// callback
int		zappy_client_move_cb(zappy_client_t *client);
int		zappy_avance_cb(zappy_client_t *client);
int		zappy_droite_cb(zappy_client_t *client);
int		zappy_gauche_cb(zappy_client_t *client);
int		zappy_voir_cb(zappy_client_t *client);
int		zappy_prend_cb(zappy_client_t *client);
int		zappy_pose_cb(zappy_client_t *client);
int		zappy_expulse_cb(zappy_client_t *client);
int		zappy_broadcast_cb(zappy_client_t *client);
int		zappy_incantation_cb(zappy_client_t *client);
int		zappy_fork_cb(zappy_client_t *client);

// callback connect_nbr
int		zappy_connect_nbr_cb(zappy_client_t *client);

// callback inventaire
int		zappy_inventaire_cb(zappy_client_t *client);
void	serialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE], t_inventaire inventaire);
void	deserialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE], t_inventaire *inventaire);
void	update_team_inventaire(zappy_client_t *client);
void	print_inventaire(t_inventaire inventaire);


#endif

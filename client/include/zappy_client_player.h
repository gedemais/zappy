#ifndef ZAPPY_CLIENT_PLAYER_H
# define ZAPPY_CLIENT_PLAYER_H


# include <stdint.h>
# include <string.h>


# define CLIENT_BUFSIZE		4096

# define PLAYER_MAX			6
# define LVL_MAX			8

# define VISION_MAP_MAX		81
# define CASE_ELEMENTS		8


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

enum			e_player_task {
	PLAYER_TASK_WAIT,
	PLAYER_TASK_ID,
	PLAYER_TASK_LOOK,
	PLAYER_TASK_LOOT,
	PLAYER_TASK_BROADCAST,
	PLAYER_TASK_BROADCAST_INVENTAIRE,
	PLAYER_TASK_GET_INVENTAIRE,
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
	int		player;
}				t_inventaire;

typedef struct	s_zappy_player
{
	int				pos_x; // Absolute position, unused ?
	int				pos_y; // Absolute position, unused ?
	uint8_t			id, lvl;
	uint8_t			vision_map[VISION_MAP_MAX * CASE_ELEMENTS];
	uint8_t			relative_pos; // Relative position for vision map
	uint8_t			relative_orientation; // Relative orientation, always start at 0 when see
									  // The absolute orientation is not known of client
									  // At each "see" the orientation is back to 0
	t_inventaire	inventaire[PLAYER_MAX];
	uint8_t			broadcast;	// permet d'identifier le type de broadcast a effectuer
	uint8_t			broadcast_msg[CLIENT_BUFSIZE]; // stock le broadcast_msg
	bool			alive;
}				t_player;

typedef struct	s_zappy_team
{
	uint8_t			size; // team max size
	uint8_t			nb_player; // current nb of players
	char			*name;
	t_inventaire	inventaire;
}				t_team;


#endif

#ifndef ZAPPY_CLIENT_PLAYER_H
# define ZAPPY_CLIENT_PLAYER_H


# include <stdint.h>
# include <string.h>


# define CLIENT_BUFSIZE		4096

# define PLAYER_MAX			6
# define LVL_MAX			8

# define VISION_MAP_MAX		81
# define CASE_ELEMENTS		8


enum			e_player_task {
	PLAYER_TASK_WAIT,
	PLAYER_TASK_ID,
	PLAYER_TASK_LOOK,
	PLAYER_TASK_LOOT,
	PLAYER_TASK_BROADCAST,
	PLAYER_TASK_GET_INVENTAIRE,
	PLAYER_TASK_INCANTATION,
	PLAYER_TASK_MAX
};

enum			e_broadcast
{
	BROADCAST_NONE,
	BROADCAST_INVENTAIRE,
	BROADCAST_PLAYER,
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

static t_inventaire	requirements[LVL_MAX] = {
	[0] = {0},
	/* lvl 1-2 */[1] = {.player=1,.linemate=1,.deraumere=0,.sibur=0,.mendiane=0,.phiras=0,.thystame=0},
	/* lvl 2-3 */[2] = {.player=2,.linemate=1,.deraumere=1,.sibur=1,.mendiane=0,.phiras=0,.thystame=0},
	/* lvl 3-4 */[3] = {.player=4,.linemate=2,.deraumere=0,.sibur=1,.mendiane=0,.phiras=2,.thystame=0},
	/* lvl 4-5 */[4] = {.player=4,.linemate=1,.deraumere=1,.sibur=2,.mendiane=0,.phiras=1,.thystame=0},
	/* lvl 5-6 */[5] = {.player=4,.linemate=1,.deraumere=2,.sibur=1,.mendiane=3,.phiras=0,.thystame=0},
	/* lvl 6-7 */[6] = {.player=6,.linemate=1,.deraumere=2,.sibur=3,.mendiane=0,.phiras=1,.thystame=0},
	/* lvl 7-8 */[7] = {.player=6,.linemate=2,.deraumere=2,.sibur=2,.mendiane=2,.phiras=2,.thystame=1}
};

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

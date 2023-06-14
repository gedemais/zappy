#ifndef ZAP_H
# define ZAP_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "zap_map.h"
#include "zap_profile.h"
#include "zap_com.h"
// #include "zap_player.h"
// #include "zap_team.h"
// #include "zap_vision.h"

typedef struct	stuff_s
{
	uint32_t nourriture;
	uint32_t linemate;
	uint32_t deraumere;
	uint32_t sibur;
	uint32_t mendiane;
	uint32_t phiras;
	uint32_t thystame;
} stuff_t;

typedef struct	player_s
{
	stuff_t			stuff;
	uint32_t		ttl;
	uint8_t			id;
}		player_t;

typedef struct broadcast_s
{
	char		*msg;
	uint32_t	msg_len;
	uint8_t		sender_id;
	uint32_t	direction;
	// TODO add parsed msg specific info (id, object, ...)
	list_t *lst;
} broadcast_t;

typedef struct team_s
{
#define TEAM_MAX_PLAYERS 6
	player_t	players[TEAM_MAX_PLAYERS];
	list_t		broadcast_history;
} team_t;

typedef struct coord_s
{
	/*
	**	This coordinate point is updated at each
	**	movement since the beginning
	**	Team using broadcast can compute their absolute position
	**	0 ----> max_x
	**	|
	**	|
	**      max_y
	*/
	uint8_t	abs_direction; // abs direction if we're starting looking at the north
	point_t abs_pos; // pos_x and pos_y are updated from the pos 0:0 with CARDINAL_N direction
			// should be usefull to calculate pos from direction
} coord_t;

// Ok alors un move de loot doit respecter ces regles :
// 1. Ne doit pas sortir de la vision map
// 2. Toujours aller au plus rapide
// 3. Si mouvement a faire > vision_size, relance un voir
typedef struct vision_s
{
	// maybe should rename
	coord_t		coord;
	uint32_t	rel_dir; // GET_INDEX_FROM_RELATIVE_POS(pos_x, pos_y)
	uint32_t	rel_y; // 
	uint32_t	rel_x;
	uint32_t	size;
#define MAX_VISION_SIZE 127
	case_t		c[MAX_VISION_SIZE]; // index of case is GET_INDEX_FROM_RELATIVE_POS(pos_x, pos_y)
	bool		in;
} vision_t;

typedef struct zap_s
{
	team_t team;
	player_t player;
	vision_t vision;
	coord_t coord;
	uint32_t max_x;
	uint32_t max_y;
	list_t 	profile;
	com_t 	com;
	uint8_t	status;
} zap_t;

int 	compute_rel_move(uint32_t relpos, int *rel_x, int *rel_y);
int 	compute_abs_move(uint32_t *x_move, uint32_t *y_move, uint32_t direction);
int	zap_move_direction(zap_t *zap, uint8_t dir);
int	zap_move_rel_coord(zap_t *zap, int tgt_x, int tgt_y);;
int		zap(zap_opt_t *opt);

#endif

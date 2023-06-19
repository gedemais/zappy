#ifndef ZAP_H
# define ZAP_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "zap_com.h"
// #include "zap_player.h"
// #include "zap_team.h"
// #include "zap_vision.h"

#include "list.h"

#include "zap_map.h"

typedef struct profile_s profile_t;
typedef int		(*zap_profile_init_t)(profile_t *profile);
typedef uint8_t		(*zap_profile_prio_t)(profile_t *profile);
typedef int		(*zap_profile_fsm_t)(profile_t *profile);

typedef struct zap_s zap_t;

typedef struct profile_s {
	zap_profile_fsm_t	fsm_cb;
	zap_profile_prio_t	prio_cb;
	zap_profile_init_t	init_cb;
	zap_t			*zap;
#define proctx (((typeof(ctx))ctx) // TODO test
	void			*ctx;
	list_t			lst;
	uint8_t			status;
	uint8_t			state;
} profile_t;


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
	list_t		broadcast_history;
#define TEAM_MAX_PLAYERS 6
	player_t	players[TEAM_MAX_PLAYERS];
} team_t;



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
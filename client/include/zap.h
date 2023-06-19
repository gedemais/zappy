#ifndef ZAP_H
# define ZAP_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "zap_req.h"
// #include "zap_player.h"
// #include "zap_team.h"
// #include "zap_vision.h"

#include "list.h"

#include "zap_map.h"
#include "zap_req.h"

typedef struct profile_s profile_t;
typedef int		(*zap_profile_init_t)(profile_t *profile);
typedef uint8_t		(*zap_profile_prio_t)(profile_t *profile);
typedef int		(*zap_profile_fsm_t)(profile_t *profile);

typedef struct zap_s zap_t;
typedef int	(*zap_req_cb_t)(req_t *);

typedef struct profile_s
{
	char			name[256];
	zap_req_cb_t		req_cb[CMD_MAX];
	zap_profile_fsm_t	fsm_cb;
	zap_profile_prio_t	prio_cb;
	zap_profile_init_t	init_cb;
	zap_t			*zap;
#define proctx (((typeof(*ctx)*)ctx) // TODO test
	void			*ctx;
	list_t			lst;
	uint8_t			status;
	uint8_t			state;
} profile_t;


typedef struct	stuff_s
{
	uint32_t content[R_MAX];
} stuff_t;

typedef struct	player_s
{
	stuff_t			stuff;
	uint32_t		ttl;
	uint8_t			id;
	bool alive;
}		player_t;

typedef struct broadcast_s
{
	char		msg[ZAP_RX_BUFSIZE];
	uint32_t	msg_len;
	uint8_t		sender_id;
	uint32_t	direction;
	// TODO add parsed msg specific info (id, object, ...)
	list_t 		lst;
} broadcast_t;

#define MAX_BROADCAST 32
typedef struct team_s
{
// TODO : team shared map
	broadcast_t		broadcast_history[MAX_BROADCAST];
#define TEAM_MAX_PLAYERS 6
	player_t	players[TEAM_MAX_PLAYERS];
	list_t	broadcast;
	list_t	broadcast_free;
} team_t;



typedef struct zap_s
{
	team_t team;
	player_t player;
	vision_t vision;
	coord_t coord;
	int32_t max_x;
	int32_t max_y;
	uint32_t time;
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

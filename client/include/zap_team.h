#ifndef ZAP_TEAM_H
# define ZAP_TEAM_H

#include "list.h"
#include "zap_player.h"

typedef struct broadcast_s
{
	char		*msg;
	uint32_t	msg_len;
	uint8_t		sender_id;
	uint32_t	direction;
	// TODO add parsed msg specific info (id, object, ...)
	list_t *lst;
} broadcast_t;

#define TEAM_MAX_PLAYERS 6
typedef struct team_s
{
	player_t	players[TEAM_MAX_PLAYERS];
	//broadcast_t	broadcast_history;
	list_t	broadcast_history;
} team_t;

typedef struct zap_s zap_t;

int zap_team_init(zap_t *zap);
void zap_team_deinit(zap_t *zap);

#endif

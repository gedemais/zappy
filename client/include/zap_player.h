#ifndef ZAP_PLAYER_H
# define ZAP_PLAYER_H

#include <stdint.h>

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

typedef struct zap_s zap_t;
int		zap_player_init(zap_t *zap);
void		zap_player_deinit(zap_t *zap);

#endif

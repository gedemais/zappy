#ifndef ZAPPY_CLIENT_H
# define ZAPPY_CLIENT_H

#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "zappy_client_getopt.h"

#define CLIENT_BUFSIZE 4096

static char see_cmd[]			= "voir";
static char broadcast_cmd[]		= "broadcast";
static char advance_cmd[]		= "avance";
static char turn_right_cmd[]	= "droite";
static char turn_left_cmd[]		= "gauche";
static char inventory_cmd[]		= "inventaire";
static char kick_cmd[]			= "expulse";
static char incantation_cmd[]	= "incantation";
static char fork_cmd[]			= "fork";
static char connect_nbr_cmd[]	= "connect_nbr";

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

#define ZAPPY_OK	0
#define ZAPPY_WAIT	1
#define ZAPPY_ERROR -1

#define CASE_ELEM_LINEMATE		0
#define CASE_ELEM_DERAUMERE		1
#define CASE_ELEM_SIBUR			2
#define CASE_ELEM_MENDIANE		3
#define CASE_ELEM_PHIRAS		4
#define CASE_ELEM_THYSTAME		5
#define CASE_ELEM_FOOD			6
#define CASE_ELEM_PLAYER		7 // TODO LMA : to remove ?

#define VISION_MAP_MAX		81
#define CASE_ELEMENTS		8

#define ORIENTATION_FRONT		0
#define ORIENTATION_RIGHT		1
#define ORIENTATION_BACK		2
#define ORIENTATION_LEFT		3

static char *case_ressources[] =
{
	"linemate",
	"deraumere",
	"sibur",
	"mendiane",
	"phiras",
	"thystame",
	"nourriture",
	"player",
	NULL
};

typedef struct zappy_client_s zappy_client_t;

typedef int (*zappy_client_cmd_cb_t)(zappy_client_t *);

typedef struct zappy_client_cmd_s
{
	char					*cmd;
	zappy_client_cmd_cb_t	cb;
} zappy_client_cmd_t;

	#define ZAPPY_CLIENT_MAX_STACKED_CMD 1

enum	e_zappy_farmer {
	ZAPPY_FARMER_LOOK = 1,
	ZAPPY_FARMER_LOOKWAIT,
	ZAPPY_FARMER_LOOT,
	ZAPPY_FARMER_MAX
};

// enum	e_zappy_farmer_broadcast {
// 	BROADCAST_INVENTORY_SEND,
// 	BROADCAST_INVENTORY_RECEIVE,
// 	BROADCAST_INVENTORY_MAX
// };

typedef struct zappy_client_s
{
	int					socket;
    struct sockaddr_in	sockaddr;
	uint8_t				buf[CLIENT_BUFSIZE];
	int					pos_x; // Absolute position, unused ?
	int					pos_y; // Absolute position, unused ?
	uint8_t				vision_map[VISION_MAP_MAX * CASE_ELEMENTS];
	uint8_t				relative_pos; // Relative position for vision map
	uint8_t				relative_orientation; // Relative orientation, always start at 0 when see
											  // The absolute orientation is not known of client
											  // At each "see" the orientation is back to 0

	zappy_client_cmd_t	cmds[ZAPPY_CLIENT_MAX_STACKED_CMD];
	uint8_t				cmd_idx; // idx used to rotate cmds
	uint8_t				cmd_stack_size; // nb of elements currently in cmds
	uint8_t				task;
} zappy_client_t;

/* main function called from main*/
int zappy_client(zappy_client_opt_t *opt);
/* transceive blocking function TODO : client should use select(2) for handle "mort" and broadcast msg */
int	zappy_client_transceive(zappy_client_t *client, char *cmd, int len, zappy_client_cmd_cb_t cb);
/* parse function for the see response TODO: inventory */
int	zappy_client_parse_see(zappy_client_t *client);
void zappy_debug_print_vision_map(zappy_client_t *client);
int	zappy_client_see_cb(zappy_client_t *client);
int zappy_client_receipt(zappy_client_t *client);

#endif

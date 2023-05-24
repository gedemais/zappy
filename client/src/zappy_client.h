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

static char see_cmd[]			= "voir\n";
static char advance_cmd[]		= "avance\n";
static char turn_right_cmd[]	= "droite\n";
static char turn_left_cmd[]	= "gauche\n";
static char inventory_cmd[]	= "inventaire\n";
static char kick_cmd[]		= "expulse\n";
static char incantation_cmd[]	= "incantation\n";
static char fork_cmd[]		= "fork\n";
static char connect_nbr_cmd[]	= "connect_nbr\n";

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

} zappy_client_t;

/* main function called from main*/
int zappy_client(zappy_client_opt_t *opt);
/* transceive blocking function TODO : client should use select(2) for handle "mort" and broadcast msg */
int	zappy_client_transceive(zappy_client_t *client, char *cmd, int len, char *expected_rsp);
/* parse function for the see response TODO: inventory */
int	zappy_client_parse_see(zappy_client_t *client);
void zappy_debug_print_vision_map(zappy_client_t *client);

#endif

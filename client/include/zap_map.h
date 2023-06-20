#ifndef ZAP_MAP_H
# define ZAP_MAP_H

#include "zap.h"

/*
TODO 
enum			e_ressources
{
	R_NOURRITURE = 0x80000000,
	R_LINEMATE = 0x40000000,
	R_DERAUMERE = 0x20000000,
	R_SIBUR = 0x10000000,
	R_MENDIANE = 0x08000000,
	R_PHIRAS = 0x04000000,
	R_THYSTAME = 0x02000000
};
typedef struct case_s {
	uint32_t content;
} case_t;
#define RESOURCES_TYPE_MSK 0xFE000000
#define RESOURCES_NB_MSK 0x01FFFFFF
#define RESOURCE_TYPE(c) ((uint32_t)(c->content & RESOURCES_TYPE_MSK))
#define RESOURCE_NB(c) ((uint32_t)(c->content & RESOURCES_NB_MSK))
*/

/* Some helpfull static const for relative vision_map array */
#define MAX_VISION_ROW 9
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
static int	vision_row_left[] = {
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
static int	vision_row_right[] = {
		0,
		3,
		8,
		15,
		22,
		33,
		46,
		61,
		78
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
	R_TTL,
	R_MAX
};

typedef struct		loot_s
{
	char	name[256];
	uint8_t	len;
	int	type;
}			loot_t;

static loot_t	ressources[R_MAX] =
{
	[R_LINEMATE]	= {.name = "linemate", .len = strlen("linemate"), .type = R_LINEMATE},
	[R_DERAUMERE]	= {.name = "deraumere", .len = strlen("deraumere"), .type = R_DERAUMERE},
	[R_SIBUR]	= {.name = "sibur", .len = strlen("sibur"), .type = R_SIBUR},
	[R_MENDIANE]	= {.name = "mendiane", .len = strlen("mendiane"), .type = R_MENDIANE},
	[R_PHIRAS]	= {.name = "phiras", .len = strlen("phiras"), .type = R_PHIRAS},
	[R_THYSTAME]	= {.name = "thystame", .len = strlen("thystame"), .type = R_THYSTAME},
	[R_PLAYER]	= {.name = "player", .len = strlen("player"), .type = R_PLAYER},
	[R_NOURRITURE]	= {.name = "nourriture", .len = strlen("nourriture"), .type = R_NOURRITURE},
	[R_TTL]	= {.name = "ttl", .len = strlen("ttl"), .type = R_TTL},
};

/* 16 cardinal points from 0 to 15, start with N */
enum cardinal_e
{
    CARDINAL_N,                  /* North (N) */
    CARDINAL_NNE,                /* North-northeast (NNE) */
    CARDINAL_NE,                 /* North-east (NE) */
    CARDINAL_ENE,                /* East-northeast (ENE) */
    CARDINAL_E,                  /* East (E) */
    CARDINAL_ESE,                /* East-southeast (ESE) */
    CARDINAL_SE,                 /* South-east (SE) */
    CARDINAL_SSE,                /* South-southeast (SSE) */
    CARDINAL_S,                  /* South (S) */
    CARDINAL_SSW,                /* South-southwest (SSW) */
    CARDINAL_SW,                 /* South-west (SW) */
    CARDINAL_WSW,                /* West-southwest (WSW) */
    CARDINAL_W,                  /* West (W) */
    CARDINAL_WNW,                /* West-northwest (WNW) */
    CARDINAL_NW,                 /* North-west (NW) */
    CARDINAL_NNW,                /* North-northwest (NNW) */
};

enum direction_e
{
    DIRECTION_FRONT = CARDINAL_N,                  /* North (N) */
    DIRECTION_RIGHT = CARDINAL_E,                  /* East (E) */
    DIRECTION_BACK = CARDINAL_S,                  /* South (S) */
    DIRECTION_LEFT = CARDINAL_W,                  /* West (W) */
};

typedef struct point_s
{
	int32_t 	pos_x;
	int32_t	pos_y;
	// case_t		*map;
} point_t;


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
			// should be usefull to calculate abs pos for team
// lazy alias
#define __dir abs_direction
#define __x abs_pos.pos_x
#define __y abs_pos.pos_y
} coord_t;

typedef struct case_s
{
	uint8_t content[R_MAX]; // idx is the enum resource_e
	coord_t coord; // coordinate of the case
	list_t lst;
} case_t;

typedef struct vision_s
{
	uint32_t	size; // current vision size
#define MAX_VISION_SIZE 128
	uint32_t	current_pos;
	case_t		c[MAX_VISION_SIZE];
	list_t		cs; // the list cs contain MAX_VISION_SIZE, it rotating with function zap_map_get_new_case
	bool		enabled;
	bool		requested;
} vision_t;

typedef struct zap_s zap_t;
typedef struct zap_opt_s zap_opt_t;

int		zap_map_init(zap_opt_t *opt, zap_t *zap);
void		zap_map_deinit(zap_t *zap);


void		zap_map_abs_droite(coord_t *coord);
void		zap_map_abs_gauche(coord_t *coord);
void		zap_map_abs_avance(coord_t *coord);
void		zap_abs_droite(zap_t *zap);
void		zap_abs_gauche(zap_t *zap);
void		zap_abs_avance(zap_t *zap);

int		zap_vision_avance(zap_t *zap);
int		zap_parse_voir(zap_t *zap);
int		zap_cmd_prepend_take_food(zap_t *zap, uint8_t food_id);
int		zap_parse_inventaire(zap_t *zap);

int		zap_move_coordinate(zap_t *zap, coord_t *coord);
int	zap_move_d_coordinate(zap_t *zap, int d_x, int d_y);
void	get_coord_dxy(zap_t *zap, int dx, int dy, coord_t *coord);
int	zap_move_d_coordinate(zap_t *zap, int d_x, int d_y);

#endif

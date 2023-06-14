#ifndef ZAP_MAP_H
# define ZAP_MAP_H


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

enum			e_ressources
{
	R_NOURRITURE,
	R_LINEMATE,
	R_DERAUMERE,
	R_SIBUR,
	R_MENDIANE,
	R_PHIRAS,
	R_THYSTAME,
	R_MAX
};
typedef struct case_s
{
	uint8_t content[R_MAX];
} case_t;

typedef struct		loot_s
{
	char	name[256];
	uint8_t	len;
	int	type;
}			loot_t;

static loot_t	ressources[R_MAX] = {
	[R_LINEMATE]	= {.name = "linemate", .len = strlen("linemate"), .type = R_LINEMATE},
	[R_DERAUMERE]	= {.name = "deraumere", .len = strlen("deraumere"), .type = R_DERAUMERE},
	[R_SIBUR]	= {.name = "sibur", .len = strlen("sibur"), .type = R_SIBUR},
	[R_MENDIANE]	= {.name = "mendiane", .len = strlen("mendiane"), .type = R_MENDIANE},
	[R_PHIRAS]	= {.name = "phiras", .len = strlen("phiras"), .type = R_PHIRAS},
	[R_THYSTAME]	= {.name = "thystame", .len = strlen("thystame"), .type = R_THYSTAME},
	[R_NOURRITURE]	= {.name = "nourriture", .len = strlen("nourriture"), .type = R_NOURRITURE},
};
// TODO use this for prend_cb and pose_cb

typedef struct point_s
{
	uint32_t 	pos_x;
	uint32_t	pos_y;
	// case_t		*map;
} point_t;

typedef struct vec_s
{
	uint32_t	direction;
	point_t		p;
} vec_t;

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

typedef struct zap_s zap_t;
typedef struct zap_opt_s zap_opt_t;
int		zap_map_init(zap_opt_t *opt, zap_t *zap);
void		zap_map_deinit(zap_t *zap);

void		zap_abs_droite(zap_t *zap);
void		zap_abs_gauche(zap_t *zap);
void		zap_abs_avance(zap_t *zap);

#endif

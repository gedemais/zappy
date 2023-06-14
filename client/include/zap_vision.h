#ifndef ZAP_VISION_H
# define ZAP_VISION_H

#include "zap_map.h"
#include <stdbool.h>

typedef struct vision_s
{
	// TODO map is the starting point  of the see here,
	// maybe should rename
#define MAX_VISION_SIZE 127
	case_t		c[MAX_VISION_SIZE];
	uint32_t	rel_pos;
	uint32_t	size;
	bool	in;
} vision_t;


int	zap_vision_add(vision_t *vision, uint32_t rel, case_t *c);
int	zap_vision_get(vision_t *vision, uint32_t rel, case_t **c);
int	zap_vision_clean(vision_t *vision, uint32_t rel);

int	zap_vision_update_avance(vision_t *vision);
int	zap_vision_update_droite(vision_t *vision);
int	zap_vision_update_gauche(vision_t *vision);


#define DEBUG_SEE_SIZE 3
int	zap_parse_voir(vision_t *vision, com_t *com);

#define DECLARE_STORAGE(nb, next, type) \
typedef struct storage_## nb_t { \
	## type elem;\
	storage_## next_t 	n[## nb]; \
};

/*
** Relative map functions utility
** Can get content, clear, add in case for a given relpos
** param: vision , use the coordinate and position inside
*/
typedef struct com_s com_t;

int	zap_parse_voir(vision_t *vision, com_t *com);

int	zap_move_direction(zap_t *zap, uint8_t dir);
int	zap_move_rel_coord(zap_t *zap, int tgt_x, int tgt_y);

#endif

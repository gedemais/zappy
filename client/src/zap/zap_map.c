#include "zap.h"
#include "zap_getopt.h"
#include "zap_com.h"
#include <stdbool.h>

void		zap_abs_droite(zap_t *zap)
{
	zap->abs_direction = ((zap->abs_direction + 4) % 16);
}
void		zap_abs_gauche(zap_t *zap)
{
	zap->abs_direction = (zap->abs_direction == 0 ? 12 : (zap->abs_direction - 4) % 16);
}
void		zap_abs_avance(zap_t *zap)
{
	switch (zap->abs_direction)
	{
		case (CARDINAL_W): {
			if (zap->abs_pos.pos_x == 0) { zap->abs_pos.pos_x = zap->max_x - 1; }
			else { zap->abs_pos.pos_x--; }
			break ;
			} 
		case (CARDINAL_E): {
			if (zap->abs_pos.pos_x == zap->max_x - 1) { zap->abs_pos.pos_x = 0; }
			else { zap->abs_pos.pos_x++; }
			break ;
			} 
		case (CARDINAL_S): {
			if (zap->abs_pos.pos_y == zap->max_y - 1) { zap->abs_pos.pos_y = 0; }
			else { zap->abs_pos.pos_y++; }
			break ;
			} 
		case (CARDINAL_N): {
			if (zap->abs_pos.pos_y == 0) { zap->abs_pos.pos_y = zap->max_y - 1; }
			else { zap->abs_pos.pos_y--; }
			break ;
			}
	}
}


/*
void	zap_case_add(case_t *c1, case_t *c2)
{
	for (int i = 0 ; i < R_MAX ; i++) {
		c1->content[i] += c2->content[i];
	}
}

int		zap_map_init(zap_opt_t *opt, zap_t *zap)
{
	int r = 0;

	vision_t *vision = NULL;
	case_t *c = malloc(sizeof(case_t) * opt->max_x * opt->max_y);
	if (!c) {r = -1; }
	else {
		map_t *map = malloc(sizeof(map_t));
		if (!map) {
			free(c);
			r = -1;
		}
		if (r == 0) {
			map->max_x = opt->max_x;
			map->max_y = opt->max_y;
			map->map = NULL;
			map->pos_x = 0;
			map->pos_y = 0;
			map->direction = CARDINAL_N;
			map->map = c;
			zap->map = map;
			if (!(vision = malloc(sizeof(vision_t)))) {
				r = -1;
			}
		}
		if (r == 0) {
			zap->vision = vision;
			memcpy(&zap->vision->map, map, sizeof(map_t));
			vision->rel_pos = 0;
			vision->rel_direction = CARDINAL_N;
		}
	}
	return (r);
}

void		zap_map_deinit(zap_t *zap)
{

	if (zap->map) {
		if (zap->map->map) {
			free(zap->map->map);
		}
		free(zap->map);
	}
}
*/

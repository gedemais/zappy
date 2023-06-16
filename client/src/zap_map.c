#include "zap.h"
#include "zap_getopt.h"
#include <stdbool.h>

// TODO vision functions :
//	fill
//	move -> check in/out, check enabled/disabled, check vision already requested
//		if out && enabled && not previously requested request a see

void		zap_map_abs_droite(coord_t *coord)
{
	coord->abs_direction = ((coord->abs_direction + 4) % 16);
}
void		zap_map_abs_gauche(coord_t *coord)
{
	coord->__dir = (coord->abs_direction == 0 ? 12 : (coord->abs_direction - 4) % 16);
}

void		zap_abs_droite(zap_t *zap)
{
	zap->coord.abs_direction = ((zap->coord.abs_direction + 4) % 16);
#ifdef VERBOSE
	fprintf(stderr, "%s:%d dir switch to abs_direction=%d\n", 
					__func__,
					__LINE__,
					zap->coord.__dir);
#endif
}
void		zap_abs_gauche(zap_t *zap)
{
	zap->coord.abs_direction = (zap->coord.abs_direction == 0 ? 12 : (zap->coord.abs_direction - 4) % 16);
#ifdef VERBOSE
	fprintf(stderr, "%s:%d dir switch to abs_direction=%d\n", 
					__func__,
					__LINE__,
					zap->coord.__dir);
#endif
}
void		zap_abs_avance(zap_t *zap)
{
	switch (zap->coord.abs_direction)
	{
		case (CARDINAL_W): {
			if (zap->coord.__x == 0) { zap->coord.__x = zap->max_x - 1; }
			else { zap->coord.__x--; }
			break ;
			} 
		case (CARDINAL_E): {
			if (zap->coord.__x == zap->max_x - 1) { zap->coord.__x = 0; }
			else { zap->coord.__x++; }
			break ;
			} 
		case (CARDINAL_S): {
			if (zap->coord.__y == zap->max_y - 1) { zap->coord.__y = 0; }
			else { zap->coord.__y++; }
			break ;
			} 
		case (CARDINAL_N): {
			if (zap->coord.__y == 0) { zap->coord.__y = zap->max_y - 1; }
			else { zap->coord.__y--; }
			break ;
			}
	}
#ifdef VERBOSE
	fprintf(stderr, "%s:%d moved to abs_direction=%d posX=%d posY=%d\n", 
					__func__,
					__LINE__,
					zap->coord.__dir, 
					zap->coord.__x,
					zap->coord.__y);
#endif
}

/*
void		zap_abs_droite(zap_t *zap)
{
	zap->coord.__dir = ((zap->coord.__dir + 4) % 16);
}
void		zap_abs_gauche(zap_t *zap)
{
	zap->coord.__dir = (zap->coord.__dir == 0 ? 12 : (zap->coord.__dir - 4) % 16);
}
void		zap_abs_avance(zap_t *zap)
{
	switch (zap->coord.__dir)
	{
		case (CARDINAL_W): {
			fprintf(stderr, "%s:%d WEST\n", __func__, __LINE__);
			if (zap->coord.__x == 0) { zap->coord.__x = zap->max_x - 1; }
			else { zap->coord.__x--; }
			break ;
			} 
		case (CARDINAL_E): {
			fprintf(stderr, "%s:%d EAST\n", __func__, __LINE__);
			if (zap->coord.__x == zap->max_x - 1) { zap->coord.__x = 0; }
			else { zap->coord.__x++; }
			break ;
			} 
		case (CARDINAL_S): {
			fprintf(stderr, "%s:%d SOUTH\n", __func__, __LINE__);
			if (zap->coord.__y == zap->max_y - 1) { zap->coord.__y = 0; }
			else { zap->coord.__y++; }
			break ;
			} 
		case (CARDINAL_N): {
			fprintf(stderr, "%s:%d NORTH\n", __func__, __LINE__);
			if (zap->coord.__y == 0) { zap->coord.__y = zap->max_y - 1; }
			else { zap->coord.__y--; }
			break ;
			}
	}
}


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

// 0: front 1:right 2:back 3:left
int	zap_move_direction(zap_t *zap, uint8_t dir)
{
	int r = 0;

	if (!zap)
	{
		r = -1;
	}
	else
	{
		switch (dir) {
			case (CARDINAL_E): {
				r = zap_queue_cmd(zap, CMD_DROITE);
				if (r == 0) {
					r = zap_queue_cmd(zap, CMD_AVANCE);
				}
				break ;
			}
			case (CARDINAL_S): {
				r = zap_queue_cmd(zap, CMD_GAUCHE);
				if (r == 0) {
					r = zap_queue_cmd(zap, CMD_GAUCHE);
				}
				if (r == 0) {
					r = zap_queue_cmd(zap, CMD_AVANCE);
				}
				break ;
			}
			case (CARDINAL_W): {
				r = zap_queue_cmd(zap, CMD_GAUCHE);
				if (r == 0) {
					r = zap_queue_cmd(zap, CMD_AVANCE);
				}
				break ;
			}
			case (CARDINAL_N): {
				r = zap_queue_cmd(zap, CMD_AVANCE);
				break ;
			}
		}
	}
	return (r);
}

int	zap_move_rel_coord(zap_t *zap, int tgt_x, int tgt_y)
{
	int r = 0;

	if (tgt_y < 0)
	{
		r = zap_move_direction(zap, DIRECTION_BACK);
			fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
		tgt_y *= -1;
		tgt_x *= -1;
		tgt_y--;
	}
	while (r == 0 && tgt_y > 0)
	{
		r = zap_move_direction(zap, DIRECTION_FRONT);
			fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
		tgt_y--;
	}
	if (r == 0 && tgt_x < 0)
	{
		r = zap_move_direction(zap, DIRECTION_LEFT);
			fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
		tgt_x++;
		while ((r == 0 && tgt_x < 0)) {
			r = zap_move_direction(zap, DIRECTION_FRONT);
			tgt_x++;
		}
	}
	if (r == 0 && tgt_x > 0)
	{
		r = zap_move_direction(zap, DIRECTION_RIGHT);
			fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
		tgt_x--;
		while ((r == 0 && tgt_x > 0)) {
			r = zap_move_direction(zap, DIRECTION_FRONT);
			fprintf(stderr, "%s:%d r=%d\n", __func__, __LINE__, r);
			tgt_x--;
		}
	}
	return (r);
}


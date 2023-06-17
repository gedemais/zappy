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

void		zap_map_vision_avance(zap_t *zap)
{
	// if enabled
	// if in
	// int rel_dir = ((zap->vision.coord.__dir - zap->coord.__dir) % 16);
	// TODO ??
	// player is in vision, we can move current_case 
	fprintf(stderr, "%s: [BEFORE UPDATE] abs dir=%d vision_dir=%d current_pos=%d\n", __func__, zap->coord.__dir, zap->vision.coord.__dir,
			zap->vision.current_pos);
	if (zap->coord.__dir == zap->vision.coord.__dir) {
		// front
		int rel_x = (zap->vision.coord.__x - zap->coord.__x) % zap->max_x;
		int rel_y = (zap->vision.coord.__y - zap->coord.__y) % zap->max_y;
		int row = 0;
		// 1. check if going out (need to have the vision.size)
		for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
			if (vision_row_right[i] <= (int)zap->vision.current_pos
				&& vision_row_left[i] >= (int)zap->vision.current_pos) {
				row = i;
			}
		}
		fprintf(stderr, "%s: rel_x=%d rel_y=%d found row=%d\n", __func__, rel_x, rel_y, row);
		if (zap->vision.current_pos + (vision_row_size[row+1] - 1) > zap->vision.size) {
			zap->vision.in = false;
		}
		else {
			zap->vision.current_pos += (uint32_t)(vision_row_size[row+1] - 1);
			fprintf(stderr, "[AFTER UPDATE] vision current_pos %d\n", zap->vision.current_pos);
		}
	}
	else if (zap->coord.__dir == (zap->vision.coord.__dir - 4) % 16) {
		// left
		// 1. check if going out : if current pos is on the left vision (0, 1, 4, 9, ...)
		// 2. easy : current_pos--;
		int row = 0;
		for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
			if (vision_row_left[i] == (int)zap->vision.current_pos) {
				row = i;
			}
		}
		if (row) {
			zap->vision.in = false;
			fprintf(stderr, "update vision : goin out\n");
		}
		else  {
			zap->vision.current_pos--;
			fprintf(stderr, "update vision current pos = %d\n", zap->vision.current_pos);
		}
	}
	else if (zap->coord.__dir == (zap->vision.coord.__dir + 4) % 16) {
		// right
		// 1. check if going out : if current pos is on the right vision (0, 3, 8, ...)
		// 2. easy : current_pos++;
		int row = 0;
		for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
			if (vision_row_right[i] == (int)zap->vision.current_pos) {
				row = i;
			}
		}
		if (row) {
			zap->vision.in = false;
			fprintf(stderr, "update vision : goin out\n");
		}
		else  {
			zap->vision.current_pos++;
			fprintf(stderr, "update vision current pos = %d\n", zap->vision.current_pos);
		}
	}
	else {
		// TODO
		// back
		// 1. check if going out : if current pos is on the right vision (0, 3, 8, ...) OR
		//			the left vision (0, 1, 4, 9, ...)
		// 2. with current_pos determine the operande
	}
}

// update current_case of vision_s struct
int		zap_vision_avance(zap_t *zap)
{
	int r = 0;
	if (zap->vision.enabled == true)
	{
		if (zap->vision.in == true) {
			fprintf(stderr, "%s ============= \n", __func__);
			zap_map_vision_avance(zap);
			fprintf(stderr, "%s ============= \n", __func__);
		}
		else {
			if (zap->vision.requested == false) {
				r = zap_queue_cmd_prepend(zap, CMD_VOIR);
				zap->vision.requested = true;
			}
		}
	}
	return (r);
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

int	zap_parse_voir(zap_t *zap)
{
	int r = 0;
	int i = 0;
	int c = 0;
	case_t cse = {0};


	if (zap->com.buf_rx[i++] != '{') {
		r = -1;
	}
	while (r == 0 && i < zap->com.buf_rx_len)
	{
		/* ' ' are ignored */
		/* , move to next case */
		/* } close */
		if (zap->com.buf_rx[i] == '}') {
			break ;
		}
		else if (zap->com.buf_rx[i] == ',') {
			c++;
			i++;
		}
		else if (zap->com.buf_rx[i] == ' ') {
			i++;;
		}
		else
		{
			bool b = false;
			for (int j = 0; j < R_MAX && b == false; j++) {
				if (!memcmp(ressources[j].name,
					(char*)&zap->com.buf_rx[i],
						ressources[j].len)) {
				b = true;
#ifdef VERBOSE
	 			fprintf(stderr, "%s:%d adding case[%d] resources=%s\n", __func__, __LINE__,
				c, ressources[j].name);
#endif
					i += ressources[j].len;
					zap->vision.c[c].content[j]++;
				}
			}
			if (b == false) {
				// can be anything
				fprintf(stderr, "%s:%d error unknow word buf={%s}\n", __func__, __LINE__, (char*)&zap->com.buf_rx[i]);
				r = -1;
			}
		}
	}
#ifdef VERBOSE
	 fprintf(stderr, "%s:%d i=%d len=%d r=%d\n", __func__, __LINE__, i, zap->com.buf_rx_len, r);
#endif
	zap->vision.requested = false;
	zap->vision.size = c + 1;
	zap->vision.current_pos = 0;
	return (r);
}


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
//static void		zap_map_vision_avance(zap_t *zap)
//{
//	(void)zap;
//	// if enabled
//	// if in
//	// int rel_dir = ((zap->vision.coord.__dir - zap->coord.__dir) % 16);
//	// TODO ??
//	// player is in vision, we can move current_case 
//	//fprintf(stderr, "%s: [BEFORE UPDATE] abs dir=%d vision_dir=%d current_pos=%d\n", __func__, zap->coord.__dir, zap->vision.coord.__dir,
//	//		zap->vision.current_pos);
//	//if (zap->coord.__dir == zap->vision.coord.__dir) {
//	//	// front
//	//	int rel_x = (zap->vision.coord.__x - zap->coord.__x) % zap->max_x;
//	//	int rel_y = (zap->vision.coord.__y - zap->coord.__y) % zap->max_y;
//	//	int row = 0;
//	//	// 1. check if going out (need to have the vision.size)
//	//	for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
//	//		if (vision_row_right[i] <= (int)zap->vision.current_pos
//	//			&& vision_row_left[i] >= (int)zap->vision.current_pos) {
//	//			row = i;
//	//		}
//	//	}
//	//	fprintf(stderr, "%s: rel_x=%d rel_y=%d found row=%d\n", __func__, rel_x, rel_y, row);
//	//	if (zap->vision.current_pos + (vision_row_size[row+1] - 1) > zap->vision.size) {
//	//		zap->vision.in = false;
//	//	}
//	//	else {
//	//		zap->vision.current_pos += (uint32_t)(vision_row_size[row+1] - 1);
//	//		fprintf(stderr, "[AFTER UPDATE] vision current_pos %d\n", zap->vision.current_pos);
//	//	}
//	//}
//	//else if (zap->coord.__dir == (zap->vision.coord.__dir - 4) % 16) {
//	//	// left
//	//	// 1. check if going out : if current pos is on the left vision (0, 1, 4, 9, ...)
//	//	// 2. easy : current_pos--;
//	//	for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
//	//		if (vision_row_left[i] == (int)zap->vision.current_pos) {
//	//			zap->vision.in = false;
//	//			fprintf(stderr, "update vision : goin out\n");
//	//			break ;
//	//		}
//	//	}
//	//	if (zap->vision.in) {
//	//		zap->vision.current_pos--;
//	//		fprintf(stderr, "[AFTER UPDATE] vision current_pos %d\n", zap->vision.current_pos);
//	//	}
//	//}
//	//else if (zap->coord.__dir == (zap->vision.coord.__dir + 4) % 16) {
//	//	// right
//	//	// 1. check if going out : if current pos is on the right vision (0, 3, 8, ...)
//	//	// 2. easy : current_pos++;
//	//	for (int i = 0 ; i < MAX_VISION_ROW ; i++) {
//	//		if (vision_row_right[i] == (int)zap->vision.current_pos) {
//	//			zap->vision.in = false;
//	//			fprintf(stderr, "update vision : goin out\n");
//	//			break ;
//	//		}
//	//	}
//	//	if (zap->vision.in) {
//	//		zap->vision.current_pos++;
//	//		fprintf(stderr, "[AFTER UPDATE] vision current_pos %d\n", zap->vision.current_pos);
//	//	}
//	//}
//	//else {
//	//	// TODO
//	//	// back
//	//	// 1. check if going out : if current pos is on the right vision (0, 3, 8, ...) OR
//	//	//			the left vision (0, 1, 4, 9, ...)
//	//	// 2. with current_pos determine the operande
//	//}
//}
//
int		zap_vision_avance(zap_t *zap)
{
	int r = 0;
	if (zap->vision.enabled == true)
	{
		fprintf(stderr, "%s: vision enabled\n", __func__);
	}
	return (r);
}

*/
#define ABS(expr) ((expr) < 0 ? ((expr) * -1) : (expr))

int	zap_move_d_coordinate(zap_t *zap, int d_x, int d_y)
{
	int r = 0;
	int dir = zap->coord.__dir;
	zap_queue_cmd(zap, CMD_VOIR);
	zap_queue_cmd(zap, CMD_INVENTAIRE);
	while ( d_y > 0 ) {
		zap_queue_cmd(zap, CMD_AVANCE);
		d_y--;
	}
	if (d_x < 0) {
		zap_queue_cmd(zap, CMD_GAUCHE);
		while (d_x < 0) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_x++;
		}
	}
	if (d_x > 0) {
		zap_queue_cmd(zap, CMD_DROITE);
		while (d_x > 0) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_x--;
		}
	}
	if (d_y < 0) {
		fprintf(stderr, "ERROR, should not do that");
		zap_queue_cmd(zap, CMD_DROITE);
		zap_queue_cmd(zap, CMD_DROITE);
		while (d_y < 0) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_y++;
		}
	}
	return (r);
}

int	zap_move_coordinate(zap_t *zap, coord_t *coord)
{
	int r = 0;
	int d_x =  coord->__x - zap->coord.__x;
	fprintf(stderr, "d_x=%d\n", d_x);
	if (ABS(d_x) > zap->max_x / 2) {
		int s = (d_x < 0 ? 1 : -1);
		d_x = zap->max_x - ABS(d_x);
		// d_x = zap->max_x - (ABS(d_x) - ((zap->max_x/2) - ABS(d_x)));
		d_x *= s;
	}
	fprintf(stderr, "d_x=%d\n", d_x);
	int d_y =  coord->__y - zap->coord.__y;
	fprintf(stderr, "d_y=%d\n", d_y);
	if (ABS(d_y) > zap->max_y / 2) {
		int s = (d_y < 0 ? 1 : -1);
		d_y = zap->max_y - ABS(d_y);
		// d_y = zap->max_y - (ABS(d_y) - ((zap->max_y/2) - ABS(d_y)));
		d_y *= s;
	}
	fprintf(stderr, "d_y=%d\n", d_y);
	int32_t pos_x = zap->coord.__x;
	int32_t pos_y = zap->coord.__y;
	int dir = zap->coord.__dir;
	zap_queue_cmd(zap, CMD_VOIR);
	zap_queue_cmd(zap, CMD_INVENTAIRE);
	while (!!d_x || !!d_y) {
		fprintf(stderr, "%s: current = {%d %d} currentdir=%d tgt={%d %d} head={%d %d} head_dir=%d d_x=%d d_y=%d\n",
				__func__,
				zap->coord.__x,
				zap->coord.__y,
				zap->coord.__dir,
				coord->__x,
				coord->__y,
				pos_x,
				pos_y ,
				dir,
				d_x, d_y
				);
		if ( d_x > 0 && dir == CARDINAL_E) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_x--;
		}
		else if ( d_y > 0 && dir == CARDINAL_S) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_y--;
		}
		else if ( d_x < 0 && dir == CARDINAL_W) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_x++;
		}
		else if ( d_y < 0 && dir == CARDINAL_N) {
			zap_queue_cmd(zap, CMD_AVANCE);
			d_y++;
		}
		else {
			// Do one rotation
			int right = ((dir + 4) % 16);
			int left = ((dir - 4) < 0 ? 12 : dir - 4);
			if ((d_y > 0 && right == CARDINAL_S)
				|| (d_x > 0 && right == CARDINAL_E)
				|| (d_y < 0 && right == CARDINAL_N)
				|| (d_x < 0 && right == CARDINAL_W))
			{
				dir = right;
				zap_queue_cmd(zap, CMD_DROITE);
			//	zap_queue_cmd(zap, CMD_VOIR);
				
			}
			else if ((d_y > 0  && left == CARDINAL_S)
				|| (d_x > 0  && left == CARDINAL_E)
				|| (d_y < 0  && left == CARDINAL_N)
				|| (d_x < 0 && pos_x != coord->__x && left == CARDINAL_W))
			{
				dir = left;
				zap_queue_cmd(zap, CMD_GAUCHE);
			//	zap_queue_cmd(zap, CMD_VOIR); TODO determine if we've moving out of max_see
				
			}
			else if (!!d_y || !!d_x) {
				// reverse 
				dir = (dir + 8) % 16;
				zap_queue_cmd(zap, CMD_GAUCHE);
				zap_queue_cmd(zap, CMD_GAUCHE);
			}
		}
	}
	return (r);
}

case_t *zap_map_get_new_case(zap_t *zap)
{
	case_t *cs = list_last_entry(&zap->vision.cs, case_t, lst);
	fprintf(stderr, "%s: [ID=%d] alloc case=%p\n", __func__, zap->player.id, cs);
	// memset(cs->content, 0, R_MAX);
	fprintf(stderr, "%s: [ID=%d] alloc case=%p lst=%p lst->next=%p lst->prev=%p\n", __func__, zap->player.id, cs, &cs->lst,
					cs->lst.next, cs->lst.prev);
	memset(cs->content, 0, R_MAX);
	list_del(&cs->lst);
	fprintf(stderr, "%s: [ID=%d] alloc case=%p\n", __func__, zap->player.id, cs);
	list_add(&cs->lst, &zap->vision.cs);
	fprintf(stderr, "%s: [ID=%d] alloc case=%p\n", __func__, zap->player.id, cs);
	// list_add_tail(&cs->lst, &zap->vision.cs);
	return (cs);
}

void	get_coord_dxy(zap_t *zap, int dx, int dy, coord_t *coord)
{
	memcpy(coord, &zap->coord, sizeof(coord_t));
	switch (zap->coord.__dir) {
		case CARDINAL_S:
			coord->__x = zap->coord.__x - dx;
			coord->__y = zap->coord.__y + dy;
			break;
		case CARDINAL_W:
			coord->__x = zap->coord.__x - dy;
			coord->__y = zap->coord.__y - dx;
			break;
		case CARDINAL_N:
			coord->__x = zap->coord.__x + dx;
			coord->__y = zap->coord.__y - dy;
			break;
		case CARDINAL_E:
			coord->__x = zap->coord.__x + dy;
			coord->__y = zap->coord.__y + dx;
			break;
	}
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
	coord->__x %= 30;
	coord->__y %= 30;
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
	if (coord->__x < 0)
		coord->__x += 30;
	if (coord->__y < 0)
		coord->__y += 30;
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
}
static void	get_coord_zappy_relpos(zap_t *zap, int relpos, coord_t *coord)
{
	int dx = 0, dy = 0;

	for (int i = 0 ; i < MAX_VISION_ROW - 1 && vision_row_left[i] <= relpos; i++) {
		if (vision_row_right[i] >= relpos) {
			dx = relpos - vision_row_center[i];
			dy = i;
			break ;
		}
	}
	switch (zap->coord.__dir) {
		case CARDINAL_S:
			coord->__x = zap->coord.__x - dx;
			coord->__y = zap->coord.__y + dy;
			break;
		case CARDINAL_W:
			coord->__x = zap->coord.__x - dy;
			coord->__y = zap->coord.__y - dx;
			break;
		case CARDINAL_N:
			coord->__x = zap->coord.__x + dx;
			coord->__y = zap->coord.__y - dy;
			break;
		case CARDINAL_E:
			coord->__x = zap->coord.__x + dy;
			coord->__y = zap->coord.__y + dx;
			break;
	}
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
	coord->__x %= 30;
	coord->__y %= 30;
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
	if (coord->__x < 0)
		coord->__x += 30;
	if (coord->__y < 0)
		coord->__y += 30;
	fprintf(stderr, "dx=%d dy=%d coordx=%d coordy=%d\n", dx, dy, coord->__x, coord->__y);
}

int	zap_parse_voir(zap_t *zap)
{
	int r = 0;
	int i = 0;
	int c = 0;
	case_t *cs = zap_map_get_new_case(zap);
	if (cs) {
		memcpy(&cs->coord, &zap->coord, sizeof(coord_t));
	}

	if (zap->com.buf_rx[i++] != '{') {
		r = -1;
		fprintf(stderr, "%s:%d [ERROR]\n", __func__, __LINE__);
	}
	while (r == 0 && i < zap->com.buf_rx_len)
	{
		/* ' ' are ignored */
		/* , move to next case */
		/* } close */
		if (zap->com.buf_rx[i] == '}') {
			if (cs) {
				get_coord_zappy_relpos(zap, c, &cs->coord);
				fprintf(stderr, "%s: [ID=%d] adding new case coordinate relpos=%d coord={%d %d} current={%d %d}\n",
					__func__,
					zap->player.id, c,
					cs->coord.__x, cs->coord.__y,
					zap->coord.__x, zap->coord.__y);
			}
			break ;
		}
		else if (zap->com.buf_rx[i] == ',') {
			if (cs) {
				get_coord_zappy_relpos(zap, c, &cs->coord);
				fprintf(stderr, "%s: [ID=%d] adding new case coordinate relpos=%d coord={%d %d} current={%d %d}dir=%d\n",
					__func__,
					zap->player.id, c,
					cs->coord.__x, cs->coord.__y,
					zap->coord.__x, zap->coord.__y, zap->coord.__dir);
				cs = zap_map_get_new_case(zap);
			}
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
	 				// fprintf(stderr, "%s:%d adding relpos=%d resources=%s\n", __func__, __LINE__,
					// c, ressources[j].name);
#endif
					i += ressources[j].len;
					cs->content[j]++;
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

int	zap_parse_inventaire(zap_t *zap)
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
				/* cmp with ressources array */
				/* if match atoi the number after ' '*/
				if (!memcmp(ressources[j].name,
					(char*)&zap->com.buf_rx[i],
						ressources[j].len)) {
				b = true;
					i += ressources[j].len;
					if ((char)zap->com.buf_rx[i] != ' ') {
						return (-1);
					}
					int nb = atoi((char*)&zap->com.buf_rx[i]);
#ifdef VERBOSE
	 			fprintf(stderr, "%s:%d [ID=%d] set %d of resources=%s\n", __func__, __LINE__,
					zap->player.id,
				nb, ressources[j].name);
#endif
					zap->player.stuff.content[j] = nb;
					while (i < ZAP_RX_BUFSIZE && zap->com.buf_rx[i] != ',' && zap->com.buf_rx[i] != '}') {
						i++;
					}
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

int	zap_cmd_prepend_take_food(zap_t *zap, uint8_t food_id)
{
	req_t *req = zap_get_req(zap);
	if (!req || food_id > R_MAX) {
		return -1;
	}
	req->zap = zap;
	req->profile = NULL;
	req->cb = NULL;
	INIT_LIST_HEAD(&req->lst);
	req->io_len = snprintf(req->buf, ZAP_RX_BUFSIZE, "%s %s", commands[CMD_PREND].name, ressources[food_id].name);
	req->cmd_id = CMD_PREND;
	zap_queue_reqlst_prepend(zap, req);
	// zap->vision.c[zap->vision.current_pos].content[food_id]--;
	return (0);
}


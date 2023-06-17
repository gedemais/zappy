#include "zap.h"
//#include "zap_vision.h"

// OK this is nice to know abs position
int 	compute_abs_move(uint32_t *x_move, uint32_t *y_move, uint32_t direction)
{
	int r = 0;

	// absolute map is north oriented.
	// need to transform it
	switch (direction)
	{
		case (CARDINAL_N): {
			break;
		}
		case (CARDINAL_E): {
			// rotate 90 clockwise
			*y_move = *x_move * -1;
			*x_move = *y_move;
			break;
		}
		case (CARDINAL_W): {
			// rotate 90 counter-clockwise
			*y_move = *x_move;
			*x_move = *y_move * -1;
			break;
		}
		case (CARDINAL_S): {
			// inverse
			*x_move *= -1;
			*y_move *= -1;
			break;
		}
		default : {
			fprintf(stderr, "%s: error unexpected direction??\n", __func__);
			r = -1;
		}
	}
	return (r);
}

// OK this one is VERY great
// rel_x is the number of left/right movement and rel_y the number of avance
// from a given vec
// I TODO should do a function like :

// int create_req_move(uint32_t relpos)
// int create_n_move_rel_dir(uint8_t cardinal)

// With that i could check for every vision the nb of move needed
// and choose the lesser
// No this is just with relpos == 0 ...k

int 	compute_rel_move(uint32_t relpos, int *rel_x, int *rel_y)
{
	// first calculate how much case should be moved and in wich relative direction (left/rigth)
	int r = 0; 

	if (relpos < 1) {
		*rel_x = (int)relpos - 0;
		*rel_y = 0;
	}
	else if (relpos >= 1 && relpos < 4) {
		*rel_x = (int)relpos - 2;
		*rel_y = 1;
	}
	else if (relpos >= 4 && relpos < 9) {
		*rel_x = (int)relpos - 6;
		*rel_y = 2;
	}
	else if (relpos >= 9 && relpos < 16) {
		*rel_x = (int)relpos - 12;
		*rel_y = 3;
	}
	else if (relpos >= 16 && relpos < 25) {
		*rel_x = (int)relpos - 20;
		*rel_y = 4;
	}
	else if (relpos >= 25 && relpos < 36) {
		*rel_x = (int)relpos - 30;
		*rel_y = 5;
	}
	else if (relpos >= 36 && relpos < 49) {
		*rel_x = (int)relpos - 42;
		*rel_y = 6;
	}
	else {
		fprintf(stderr, "%s: errror unexpected relpos??\n", __func__);
		r = -1;
	}
	return (r);
}


/*
int	zap_vision_add(vision_t *vision, uint32_t rel, case_t *c)
{
	int r = 0;
	int move_x, move_y = 0;
	
	r = compute_rel_move(rel, &move_x, &move_y);
	if (r == 0) {
		r = compute_abs_move(map, &move_x, &move_y);
	}
	if (r == 0) {
		int pos_x = ((int)map->pos_x + move_x) % (int)map->max_x;
		int pos_y = ((int)map->pos_y + move_y) % (int)map->max_y;
		zap_case_add(&map->map[((uint32_t)pos_y) * map->max_y
			+ (uint32_t)pos_x], c);
	}
	return (r);
}

int	zap_vision_get(vision_t *vision, uint32_t rel, case_t **c)
{
	int r = 0;
	int move_x, move_y = 0;
	
	r = compute_rel_move(rel, &move_x, &move_y);
	if (r == 0) {
		r = compute_abs_move(vision->map, &move_x, &move_y);
	}
	if (r == 0) {
		int pos_x = ((int)vision->map->pos_x + move_x) % (int)vision->map->max_x;
		int pos_y = ((int)vision->map->pos_y + move_y) % (int)vision->map->max_y;
		*c = ((case_t*)&vision->map->map[((uint32_t)pos_y) * vision->map->max_y
			+ (uint32_t)pos_x]);
	}
	return (r);
}

int	zap_vision_clean(vision_t *vision, uint32_t rel)
{
	int r = 0;
	int move_x, move_y = 0;
	
	r = compute_rel_move(rel, &move_x, &move_y);
	if (r == 0) {
		r = compute_abs_move(vision->map, &move_x, &move_y);
	}
	if (r == 0) {
		int pos_x = ((int)vision->map->pos_x + move_x) % (int)vision->map->max_x;
		int pos_y = ((int)vision->map->pos_y + move_y) % (int)vision->map->max_y;
		memset(&vision->map->map[((uint32_t)pos_y) * vision->map->max_x
			+ (uint32_t)pos_x], 0, sizeof(case_t));
	}
	return (r);
}

int	zap_vision_clean_all(vision_t *vision)
{

}

int	zap_vision_update_avance(vision_t *vision)
{
	vision->rel_pos = 0;
}
int	zap_vision_update_droite(vision_t *vision)
{
	vision->direction = ((vision->direction + 4) % 16);
}

int	zap_vision_update_gauche(vision_t *vision)
{
	vision->direction = (vision->direction == 0 ? 12 : (vision->direction - 4) % 16);
}

*/

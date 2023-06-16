
#include "tst1.h"
#include "zap.h"

uint8_t	tst1_prio(profile_t *profile)
{
	(void)profile;
	return ((uint8_t)255);
}

int	tst1_init_cb(profile_t *profile)
{
	tst1_t *tst1_ctx = malloc(sizeof(tst1_t));
	int r = 0;
	if (!tst1_ctx)
		r = -1;
	else {
		profile->ctx = tst1_ctx;
		profile->fsm_cb = tst1_fsm;
		profile->prio_cb = tst1_prio;
		profile->state = TST1_MOVE_REL_COORD;
		tst1_ctx->init_direction = profile->zap->coord.abs_direction;
		tst1_ctx->init_pos_x = profile->zap->coord.abs_pos.pos_x;
		tst1_ctx->init_pos_y = profile->zap->coord.abs_pos.pos_y;
		tst1_ctx->target_rel_coord_x = 1;
		tst1_ctx->target_rel_coord_y = 1;
	}
	return (r);
}

int 	tst1_fsm(profile_t *profile)
{
	int		r = 0;
	req_t		*req = NULL;

	switch (profile->state)
	{

		case (TST1_MOVE_REL_COORD):
#ifdef VERBOSE
			fprintf(stderr, "TST1_MOVE_REL_COORD abs_pos_x=%d abs_pos_y=%d abs_dir=%d\n", profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y, profile->zap->coord.abs_direction);
			r = zap_move_rel_coord(profile->zap,
				((tst1_t*)profile->ctx)->target_rel_coord_x,
				((tst1_t*)profile->ctx)->target_rel_coord_y);
			fprintf(stderr, "%s:%d move_rel_coord return move to x=%d y=%d r=%d\n", __func__, __LINE__, ((tst1_t*)profile->ctx)->target_rel_coord_x, ((tst1_t*)profile->ctx)->target_rel_coord_y, r);
#endif
			if (r == 0) {
				profile->state = TST1_CHECK;
			}
			break ;
		case (TST1_CHECK): {
			int x, y;
			if (((tst1_t*)profile->ctx)->init_direction == CARDINAL_N) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)- ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)+ ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else if (((tst1_t*)profile->ctx)->init_direction == CARDINAL_E) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)+ ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)+ ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else if (((tst1_t*)profile->ctx)->init_direction == CARDINAL_W) {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)- ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)- ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			else {
				y = ((((tst1_t*)profile->ctx)->init_pos_y)+ ((tst1_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst1_t*)profile->ctx)->init_pos_x)- ((tst1_t*)profile->ctx)->target_rel_coord_x);
			}
			if (x < 0) {
				x += profile->zap->max_x;
			}
			if (y < 0) {
				y += profile->zap->max_y;
			}
			x %= profile->zap->max_x;
			y %= profile->zap->max_y;

#ifdef EXTRA_VERBOSE
			fprintf(stderr, "%s:%d TST1_CHECK abs_pos_x=%d abs_pos_y=%d abs_dir=%d init_dir=%d init_x=%d init_y=%d tgt_x=%d tgt_y=%d x=%d y=%d\n",
				__func__, 
				__LINE__, 
				profile->zap->coord.abs_pos.pos_x,
				profile->zap->coord.abs_pos.pos_y,
				profile->zap->coord.abs_direction,
				((tst1_t*)profile->ctx)->init_pos_x,
				((tst1_t*)profile->ctx)->init_pos_y,
				((tst1_t*)profile->ctx)->target_rel_coord_x,
				((tst1_t*)profile->ctx)->target_rel_coord_y,
				((tst1_t*)profile->ctx)->init_direction, x, y);
#endif
			if (profile->zap->coord.abs_pos.pos_x == (uint32_t)x
				&& profile->zap->coord.abs_pos.pos_y == (uint32_t)y)
			{
#ifdef VERBOSE
				fprintf(stderr, "%s: REACHED!! tgt_x=%d tgt_y=%d\n", __func__, ((tst1_t*)profile->ctx)->target_rel_coord_x, ((tst1_t*)profile->ctx)->target_rel_coord_y);
				((tst1_t*)profile->ctx)->init_direction = profile->zap->coord.abs_direction;
				((tst1_t*)profile->ctx)->init_pos_x = profile->zap->coord.abs_pos.pos_x;
				((tst1_t*)profile->ctx)->init_pos_y = profile->zap->coord.abs_pos.pos_y;
				((tst1_t*)profile->ctx)->target_rel_coord_x = (((tst1_t*)profile->ctx)->target_rel_coord_x + 1) % profile->zap->max_x;
				((tst1_t*)profile->ctx)->target_rel_coord_y = (((tst1_t*)profile->ctx)->target_rel_coord_y + 1) % profile->zap->max_y;
#endif
				profile->state = TST1_MOVE_REL_COORD;
				
			}
			break ;
		}
		case (TST1_WAIT):
		default:
			break ;
	}
	return (r);
}


#include "tst2.h"
#include "zap.h"

uint8_t	tst2_prio(profile_t *profile)
{
	(void)profile;
	return ((uint8_t)255);
}

int	tst2_init_cb(profile_t *profile)
{
	tst2_t *tst2_ctx = malloc(sizeof(tst2_t));
	int r = 0;
	if (!tst2_ctx)
		r = -1;
	else {
		profile->ctx = tst2_ctx;
		profile->fsm_cb = tst2_fsm;
		profile->prio_cb = tst2_prio;
		profile->state = TST2_MOVE_REL_COORD;
		tst2_ctx->init_direction = profile->zap->coord.abs_direction;
		tst2_ctx->init_pos_x = profile->zap->coord.abs_pos.pos_x;
		tst2_ctx->init_pos_y = profile->zap->coord.abs_pos.pos_y;
		tst2_ctx->target_rel_coord_x = 3;
		tst2_ctx->target_rel_coord_y = 3;
	}
	return (r);
}

int 	tst2_fsm(profile_t *profile)
{
	int		r = 0;
	req_t		*req = NULL;

	switch (profile->state)
	{

		case (TST2_MOVE_REL_COORD):
			fprintf(stderr, "TST2_MOVE_REL_COORD abs_pos_x=%d abs_pos_y=%d abs_dir=%d\n", profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y, profile->zap->coord.abs_direction);
			r = zap_move_rel_coord(profile->zap,
				((tst2_t*)profile->ctx)->target_rel_coord_x,
				((tst2_t*)profile->ctx)->target_rel_coord_y);
			fprintf(stderr, "%s:%d r=%d zap=%p\n", __func__, __LINE__, r, profile->zap);
			if (r == 0) {
				profile->state = TST2_CHECK;
			}
			break ;
		case (TST2_CHECK): {
			int x, y;
			if (((tst2_t*)profile->ctx)->init_direction == CARDINAL_N) {
				y = ((((tst2_t*)profile->ctx)->init_pos_y)- ((tst2_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst2_t*)profile->ctx)->init_pos_x)+ ((tst2_t*)profile->ctx)->target_rel_coord_x);
				fprintf(stderr, "%s:%d init_dir=%d init_x=%d init_y=%d tgt_x=%d tgt_y=%d x=%d y=%d\n",
					__func__, 
					__LINE__, 
					((tst2_t*)profile->ctx)->init_pos_x,
					((tst2_t*)profile->ctx)->init_pos_y,
					((tst2_t*)profile->ctx)->target_rel_coord_x,
					((tst2_t*)profile->ctx)->target_rel_coord_y,
					((tst2_t*)profile->ctx)->init_direction, x, y);
			}
			else if (((tst2_t*)profile->ctx)->init_direction == CARDINAL_E) {
				fprintf(stderr, "%s:%d init_dir=%d\n", __func__, __LINE__, ((tst2_t*)profile->ctx)->init_direction);
				y = ((((tst2_t*)profile->ctx)->init_pos_y)+ ((tst2_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst2_t*)profile->ctx)->init_pos_x)+ ((tst2_t*)profile->ctx)->target_rel_coord_x);
			}
			else if (((tst2_t*)profile->ctx)->init_direction == CARDINAL_W) {
				fprintf(stderr, "%s:%d init_dir=%d\n", __func__, __LINE__, ((tst2_t*)profile->ctx)->init_direction);
				y = ((((tst2_t*)profile->ctx)->init_pos_y)- ((tst2_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst2_t*)profile->ctx)->init_pos_x)- ((tst2_t*)profile->ctx)->target_rel_coord_x);
			}
			else {
				fprintf(stderr, "%s:%d init_dir=%d\n", __func__, __LINE__, ((tst2_t*)profile->ctx)->init_direction);
				y = ((((tst2_t*)profile->ctx)->init_pos_y)+ ((tst2_t*)profile->ctx)->target_rel_coord_y);
				x = ((((tst2_t*)profile->ctx)->init_pos_x)- ((tst2_t*)profile->ctx)->target_rel_coord_x);
			}
			if (x < 0) {
				x += profile->zap->max_x;
			}
			if (y < 0) {
				y += profile->zap->max_y;
			}
			x %= profile->zap->max_x;
			y %= profile->zap->max_y;

			if (profile->zap->coord.abs_pos.pos_x == (uint32_t)x
				&& profile->zap->coord.abs_pos.pos_y == (uint32_t)y)
			{
				fprintf(stderr, "%s: REACHED!! tgt_x=%d tgt_y=%d\n", __func__, ((tst2_t*)profile->ctx)->target_rel_coord_x, ((tst2_t*)profile->ctx)->target_rel_coord_y);
				((tst2_t*)profile->ctx)->init_direction = profile->zap->coord.abs_direction;
				((tst2_t*)profile->ctx)->init_pos_x = profile->zap->coord.abs_pos.pos_x;
				((tst2_t*)profile->ctx)->init_pos_y = profile->zap->coord.abs_pos.pos_y;
				((tst2_t*)profile->ctx)->target_rel_coord_x = (((tst2_t*)profile->ctx)->target_rel_coord_x + 1) % profile->zap->max_x;
				((tst2_t*)profile->ctx)->target_rel_coord_y = (((tst2_t*)profile->ctx)->target_rel_coord_y + 1) % profile->zap->max_y;
				profile->state = TST2_MOVE_REL_COORD;
				
			}
			else if (list_empty(&profile->zap->com.req_queue))
			{
				fprintf(stderr, "%s:%d ERROR not reached and no more req_queue\n", __func__, __LINE__);
				fprintf(stderr, "TST_CHECK abs_pos_x=%d abs_pos_y=%d abs_dir=%d\n", profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y, profile->zap->coord.abs_direction);
				fprintf(stderr, "%s: tgt_x=%d tgt_y=%d\n", __func__, ((tst2_t*)profile->ctx)->target_rel_coord_x, ((tst2_t*)profile->ctx)->target_rel_coord_y);
				fprintf(stderr, "%s: init_x=%d init_y=%d\n", __func__, ((tst2_t*)profile->ctx)->init_pos_x, ((tst2_t*)profile->ctx)->init_pos_y);
				fprintf(stderr, "max_x=%d max_y=%d\n", profile->zap->max_x, profile->zap->max_y);
				fprintf(stderr, "x=%d y=%d\n", x, y);
			}
			// fprintf(stderr, "%s:%d pos_x=%d pos_y=%d\n", __func__, __LINE__, profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y);
			break ;
		}
		case (TST2_WAIT):
		fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		default:
			break ;
	}
	return (r);
}

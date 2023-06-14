
#include "tst.h"
#include "zap.h"
#include "zap_profile.h"

uint8_t	tst_prio(profile_t *profile)
{
	(void)profile;
	return ((uint8_t)255);
}

int	tst_init_cb(profile_t *profile)
{
	tst_t *tst_ctx = malloc(sizeof(tst_t));
	profile->ctx = tst_ctx;
	tst_ctx->init_direction = profile->zap->coord.abs_direction;
	tst_ctx->init_pos_x = profile->zap->coord.abs_pos.pos_x;
	tst_ctx->init_pos_y = profile->zap->coord.abs_pos.pos_y;
	tst_ctx->target_rel_coord_x = 3;
	tst_ctx->target_rel_coord_y = 3;
	return (0);
}

int	tst_init(zap_t *zap)
{
	profile_t *fprofile = malloc(sizeof(profile_t));
	fprofile->fsm_cb = tst_fsm;
	fprofile->prio_cb = tst_prio;
	fprofile->init_cb = tst_init_cb;
	fprofile->status = 0;
	fprofile->state = TST_MOVE_REL_COORD;
	fprofile->zap = zap;
	INIT_LIST_HEAD(&fprofile->lst);
	fprofile->ctx = NULL; //(void*)&tst_ctx
	zap_register_profile(zap, fprofile);

int	tst_init_cb(profile_t *profile);

	return (0);
}

int 	tst_fsm(profile_t *profile)
{
	int		r = 0;
	req_t		*req = NULL;

	
	switch (profile->state)
	{
		case (TST_MOVE_REL_COORD):
			r = zap_move_rel_coord(profile->zap,
				((tst_t*)profile->ctx)->target_rel_coord_x,
				((tst_t*)profile->ctx)->target_rel_coord_y);
			if (r == 0) {
				profile->state = TST_CHECK;
			}
			else {
				fprintf(stderr, "%s:%d\n error\n", __func__, __LINE__);
			}
			break ;
		case (TST_MOVE_REL_POS):
			break;
		case (TST_CHECK):
			if (profile->zap->coord.abs_pos.pos_x == ((tst_t*)profile->ctx)->init_pos_x + ((tst_t*)profile->ctx)->target_rel_coord_x
				&& profile->zap->coord.abs_pos.pos_x == ((tst_t*)profile->ctx)->init_pos_x + ((tst_t*)profile->ctx)->target_rel_coord_x) {
				fprintf(stderr, "reached\n");
				while (1)
					;
			}
			// fprintf(stderr, "%s:%d pos_x=%d pos_y=%d\n", __func__, __LINE__, profile->zap->coord.abs_pos.pos_x, profile->zap->coord.abs_pos.pos_y);
			break ;
		case (TST_WAIT):
		default:
			break ;
	}
	return (r);
}

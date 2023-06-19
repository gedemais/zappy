
#include "tst2.h"
#include "zap.h"

uint8_t	tst2_prio(profile_t *profile)
{
	if (profile->zap->vision.c[profile->zap->vision.current_pos].content[R_NOURRITURE] > 0) {
		return (128);
	}
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
		profile->state = TST2_LOOT_FOOD;
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
		case (TST2_LOOT_FOOD):
#ifdef VERBOSE
			fprintf(stderr, "%s: [ID=%d] food current=%d\n", __func__, profile->zap->player.id, profile->zap->player.stuff.content[R_NOURRITURE]);
			fprintf(stderr, "%s:%d taking all food in case[%d]=%d\n", __func__, __LINE__, profile->zap->vision.current_pos, profile->zap->vision.c[0].content[R_NOURRITURE]);
#endif
			for (int i = 0 ; i < profile->zap->vision.c[profile->zap->vision.current_pos].content[R_NOURRITURE] ; i++) {
				zap_cmd_prepend_take_food(profile->zap, R_NOURRITURE);
			}
			break ;
		case (TST2_WAIT):
		default:
			break ;
	}
	return (r);
}

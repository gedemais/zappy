
#include "tst2.h"
#include "zap.h"

uint8_t	tst2_prio(profile_t *profile)
{
	(void)profile;
	// fprintf(stderr, "%s:%d\n", __func__, __LINE__);
	profile->zap->vision.enabled = true;
	if (profile->zap->vision.c[profile->zap->vision.current_pos].content[R_NOURRITURE] > 0) {
		fprintf(stderr, "%s: food detected, taking control\n", __func__);
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
			fprintf(stderr, "%s:%d taking all food in case0=%d\n", __func__, __LINE__, profile->zap->vision.c[0].content[R_NOURRITURE]);
			for (int i = 0 ; i < profile->zap->vision.c[profile->zap->vision.current_pos].content[R_NOURRITURE] ; i++) {
				req_t *req = zap_get_req(profile->zap);
				if (!req) {
					return -1;
				}
				req->zap = profile->zap;
				req->profile = NULL;
				req->cb = NULL;
				INIT_LIST_HEAD(&req->lst);
				req->io_len = strlen("prend nourriture");
				memcpy(req->buf, "prend nourriture", req->io_len);
				req->cmd_id = CMD_PREND;
				zap_queue_reqlst_prepend(profile->zap, req);
				profile->zap->vision.c[profile->zap->vision.current_pos].content[R_NOURRITURE]--;
			}
			break ;
		case (TST2_WAIT):
			fprintf(stderr, "%s:%d\n", __func__, __LINE__);
		default:
			break ;
	}
	return (r);
}

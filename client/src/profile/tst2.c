
#include "tst2.h"
#include "zap.h"

uint8_t	tst2_prio(profile_t *profile)
{
	case_t *cs = NULL;
	list_for_each_entry(cs, &profile->zap->vision.cs, lst) {
		if (cs->coord.__x == profile->zap->coord.__x && cs->coord.__x == profile->zap->coord.__x) {
			if (cs->content[R_NOURRITURE] > 0) {
				memcpy(&((tst2_t*)profile->ctx)->case_tgt, cs, sizeof(coord_t));
				((tst2_t*)profile->ctx)->case_p = cs;
				fprintf(stderr, "[ID=%d] detected food at pos={%d %d}\n",
					profile->zap->player.id,
					profile->zap->coord.__x, profile->zap->coord.__y);
				return (128);
			}
		}
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
		memset(&tst2_ctx->case_tgt, 0, sizeof(case_t));
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
			fprintf(stderr, "%s: [ID=%d] taking all food in tgt={%d %d} food=%d\n", __func__, profile->zap->player.id, ((tst2_t*)profile->ctx)->case_tgt.coord.__x, ((tst2_t*)profile->ctx)->case_tgt.coord.__x, ((tst2_t*)profile->ctx)->case_tgt.content[R_NOURRITURE]);
#endif
			for (int i = 0 ; i < ((tst2_t*)profile->ctx)->case_p->content[R_NOURRITURE] ; i++) {
				((tst2_t*)profile->ctx)->case_p->content[R_NOURRITURE]--;
				zap_cmd_prepend_take_food(profile->zap, R_NOURRITURE);
			}
			break ;
		case (TST2_WAIT):
		default:
			break ;
	}
	return (r);
}

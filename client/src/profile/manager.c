
#include "tst1.h"

// TODO stop init avec malloc et
// do a 1 client per program runnin' only on stack

profile_t profile_tst1 =
{
	.name = "tst1",
	.status = 0,
	.zap = NULL,
	.lst = { (void*)LIST_POISON1, (void*)LIST_POISON2 },
	.init_cb = tst1_init_cb,
	.prio_cb = tst1_prio,
	.fsm_cb = tst1_fsm
};


int	zap_profile_manager_init(zap_t *zap)
{
	profile_t *p = malloc(sizeof(profile_t));
	memcpy(p, &profile_tst1, sizeof(profile_t));
	p->zap = zap;
	list_add_tail(&p->lst, &zap->profile);
	int r = p->init_cb(p);
	return (r);
}

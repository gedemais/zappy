
#include "tst1.h"
#include "tst2.h"

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

profile_t profile_tst2 =
{
	.name = "tst2",
	.status = 0,
	.zap = NULL,
	.lst = { (void*)LIST_POISON1, (void*)LIST_POISON2 },
	.init_cb = tst2_init_cb,
	.prio_cb = tst2_prio,
	.fsm_cb = tst2_fsm
};


int	zap_profile_manager_init(zap_t *zap)
{
	int r;
	profile_t *p = malloc(sizeof(profile_t));
	memcpy(p, &profile_tst1, sizeof(profile_t));
	p->zap = zap;
	list_add_tail(&p->lst, &zap->profile);
	r = p->init_cb(p);

	p = malloc(sizeof(profile_t));
	memcpy(p, &profile_tst2, sizeof(profile_t));
	p->zap = zap;
	list_add_tail(&p->lst, &zap->profile);
	r = p->init_cb(p);
	return (r);
}

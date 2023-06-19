
#include "tst1.h"
#include "tst2.h"
#include "tst3.h"

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

profile_t profile_tst3 =
{
	.name = "tst3",
	.status = 0,
	.zap = NULL,
	.lst = { (void*)LIST_POISON1, (void*)LIST_POISON2 },
	.init_cb = tst3_init_cb,
	.prio_cb = tst3_prio,
	.fsm_cb = tst3_fsm
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

	p = malloc(sizeof(profile_t));
	memcpy(p, &profile_tst3, sizeof(profile_t));
	p->zap = zap;
	list_add_tail(&p->lst, &zap->profile);
	r = p->init_cb(p);

	return (r);
}

profile_t	*zap_get_next_profile(zap_t *zap)
{
	int i = 0;
	uint8_t prio = 255;
	profile_t *p = NULL;

	if (!list_empty(&zap->profile))
	{
		profile_t *head = NULL;
		list_for_each_entry(head, &zap->profile, lst) {
			uint8_t profile_prio = head->prio_cb(head);
			if (prio >= profile_prio) {
				prio = profile_prio;
				p = head;
			}
#ifdef EXTRAVERBOSE
	fprintf(stderr, "%s:%d profile(%d] prio=%hhu current_prio=%hhu\n",
		__func__, __LINE__, i++, profile_prio, prio);
#endif
		}
	}
#ifdef EXTRAVERBOSE
	fprintf(stderr, "%s: select profile {%s} current state = %d\n", __func__,
		p->name, p->state);
#endif
	return (p);
}

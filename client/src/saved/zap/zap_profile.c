
#include "zap.h"
#include "zap_profile.h"

#include "farmer.h"

profile_t	*zap_get_next_profile(zap_t *ctx)
{
	uint8_t	prio = 255;
	profile_t *select = NULL;
	profile_t *head = NULL;
	int i = 0;
	if (!list_empty(&ctx->profile))
	{
		list_for_each_entry(head, &ctx->profile, lst) {
			uint8_t profile_prio = head->prio_cb(head);
			if (prio >= profile_prio) {
				prio = profile_prio;
				select = head;
			}
			// fprintf(stderr, "%s:%d farmer_prio=%p head->prio_cb=%p\n", __func__, __LINE__, farmer_prio, head->prio_cb);
			fprintf(stderr, "%s:%d profile(%d] prio=%hhu current_prio=%hhu\n", __func__, __LINE__, i, profile_prio, prio);
		};
	}
	else
	{
		fprintf(stderr, "%s:%d no profile\n", __func__, __LINE__);
	}
	return (select);
}

int		zap_run_profile(zap_t *ctx)
{
	profile_t *profile = zap_get_next_profile(ctx);
	int r = 0;
	if (profile) {
		r = profile->fsm_cb(profile);
	}
	else {
		r = -1;
	}
	return (r);
}

int		zap_register_profile(zap_t *ctx, profile_t *profile)
{
	profile->zap = ctx;
	list_add(&profile->lst, &ctx->profile);
	int r = profile->init_cb(profile);
	fprintf(stderr, "adding profile to profile list r=%d\n", r);
	return (r);
}

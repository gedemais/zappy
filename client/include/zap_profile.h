#ifndef ZAP_PROFILE_H
# define ZAP_PROFILE_H

#include "list.h"

typedef struct profile_s profile_t;
typedef int		(*zap_profile_init_t)(profile_t *profile);
typedef uint8_t		(*zap_profile_prio_t)(profile_t *profile);
typedef int		(*zap_profile_fsm_t)(profile_t *profile);

typedef struct profile_s {
	zap_profile_fsm_t	fsm_cb;
	zap_profile_prio_t	prio_cb;
	zap_profile_init_t	init_cb;
	zap_t			*zap;
	void			*ctx;
	list_t			lst;
	uint8_t			status;
	uint8_t			state;
} profile_t;

profile_t	*zap_get_next_profile(zap_t *ctx);
int		zap_register_profile(zap_t *ctx, profile_t *profile);
int		zap_run_profile(zap_t *ctx);

#endif

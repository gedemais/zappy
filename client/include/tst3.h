
#ifndef TST3_H
# define TST3_H

#include "zap.h"
#include "zap_callback.h"

enum tst3_state_e
{
	TST3_YELL,
	TST3_WAIT
};

typedef struct tst3_s
{
	int flag;
} tst3_t;

uint8_t	tst3_prio(profile_t *profile);
int	tst3_init(zap_t *zap);
int 	tst3_fsm(profile_t *profile);
int	tst3_init_cb(profile_t *profile);

#endif



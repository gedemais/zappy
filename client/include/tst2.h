#ifndef TST2_H
# define TST2_H

#include "zap.h"
#include "zap_callback.h"

enum tst2_state_e
{
	TST2_LOOT_FOOD,
	TST2_WAIT
};

typedef struct tst2_s
{
	
	coord_t coord_tgt;
	case_t case_tgt;
	case_t *case_p;
} tst2_t;

uint8_t	tst2_prio(profile_t *profile);
int	tst2_init(zap_t *zap);
int 	tst2_fsm(profile_t *profile);
int	tst2_init_cb(profile_t *profile);

#endif



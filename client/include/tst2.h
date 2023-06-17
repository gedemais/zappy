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
	uint32_t 	init_direction;
	uint32_t 	init_pos_x;
	uint32_t 	init_pos_y;
	int		target_rel_coord_x; // set to 3 to move +3 on x axis
	int		target_rel_coord_y;
	uint32_t target_rel_pos; // for rel pos tests..
} tst2_t;

uint8_t	tst2_prio(profile_t *profile);
int	tst2_init(zap_t *zap);
int 	tst2_fsm(profile_t *profile);
int	tst2_init_cb(profile_t *profile);

#endif



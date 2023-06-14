#ifndef TST_H
# define TST_H

#include "zap.h"
#include "zap_callback.h"

enum tst_state_e
{
	TST_MOVE_REL_COORD,
	TST_MOVE_REL_POS,
	TST_CHECK,
	TST_WAIT
};

typedef struct tst_s
{
	uint32_t 	init_direction;
	uint32_t 	init_pos_x;
	uint32_t 	init_pos_y;
	int		target_rel_coord_x; // set to 3 to move +3 on x axis
	int		target_rel_coord_y;
	uint32_t target_rel_pos; // for rel pos tests..
} tst_t;

uint8_t	tst_prio(profile_t *profile);
int	tst_init(zap_t *zap);
int 	tst_fsm(profile_t *profile);

#endif


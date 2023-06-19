#ifndef TST1_H
# define TST1_H

#include "zap.h"
#include "zap_callback.h"

enum tst1_state_e
{
	TST1_INIT_ID,

	TST1_MOVE_D_COORDINATE,
	TST1_CHECK_COORDINATE,

	TST1_MOVE_REL_COORD,
	TST1_OLDMOVE_REL_COORD,
	TST1_CHECK,
	TST1_WAIT
};

typedef struct tst1_s
{
	uint32_t 	init_direction;
	uint32_t 	init_pos_x;
	uint32_t 	init_pos_y;
	int		target_rel_coord_x; // set to 3 to move +3 on x axis
	int		target_rel_coord_y;
	int		tgt_dir;
	uint32_t target_rel_pos; // for rel pos tests..

	struct timeval 	t_send;
	int		d_x, d_y;
	coord_t		tgt;
} tst1_t;

uint8_t	tst1_prio(profile_t *profile);
int	tst1_init(zap_t *zap);
int 	tst1_fsm(profile_t *profile);
int	tst1_init_cb(profile_t *profile);

#endif


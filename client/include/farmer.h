#ifndef FARMER_H
# define FARMER_H

#include "zap.h"
#include "zap_callback.h"

enum farmer_state_e {
	FARMER_LOOK,
	FARMER_FIND,
	FARMER_MOVE,
	FARMER_RANDOM_MOVE,
	FARMER_LOOT,
	FARMER_WAIT
};

typedef struct farmer_s {
	uint32_t rel_size;
	uint32_t rel_pos;
	uint32_t rel_tgt;
} farmer_t;

uint8_t	farmer_prio(profile_t *profile);
int	farmer_init(zap_t *zap);
int 	farmer_fsm(profile_t *profile);

#endif

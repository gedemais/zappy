#include "zap_player.h"
#include "zap.h"

int zap_player_init(zap_t *zap)
{
	int r = 0;
	player_t *player = malloc(sizeof(player));
	if (player) {
		zap->player = player;
	}
	else {
		r = -1;
	}
	return (r);
}

void zap_player_deinit(zap_t *zap)
{
	free(zap->player);
}

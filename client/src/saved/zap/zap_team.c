#include "zap_team.h"
#include "zap.h"


int zap_team_init(zap_t *zap)
{
	int r = 0;
	team_t *team = malloc(sizeof(team_t));
	if (team) {
		zap->team = team;
	}
	else {
		r = -1;
	}
	return (r);
}

void zap_team_deinit(zap_t *zap)
{
	free(zap->team);
}

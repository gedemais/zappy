#include "zappy_client.h"


// handle server's message : mort
// message: mort
int		zappy_mort(zappy_client_t *client)
{
	int	r = ZAPPY_WAIT;

	fprintf(stderr, "%s\n", __func__);
	client->player.alive = false;
	return (r);
}

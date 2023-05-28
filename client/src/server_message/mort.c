#include "zappy_client.h"


// handle server's message : mort
// message: mort
int		zappy_mort(zappy_client_t *client)
{
	(void)client;

	int	r = 1;

	client->player.alive = false;
	fprintf(stderr, "%s\n", __func__);
	return (r);
}

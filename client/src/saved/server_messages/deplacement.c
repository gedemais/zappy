#include "zappy_client.h"


// handle server's message : deplacement
// message: deplacement <K> (With K indicating the direction of the square where the player comes from)
int		zappy_deplacement(zappy_client_t *client)
{
	(void)client;

	int	r = ZAPPY_WAIT;

	fprintf(stderr, "%s\n", __func__);
	return (r);
}

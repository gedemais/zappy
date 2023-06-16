#include "zappy_client.h"


// handle server's message : niveau actuel
// message: niveau actuel : <K>
int		zappy_niveau(zappy_client_t *client)
{
	(void)client;

	int	r = ZAPPY_WAIT;

	fprintf(stderr, "%s\n", __func__);
	return (r);
}

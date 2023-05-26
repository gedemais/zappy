#include "zappy_client.h"


// handle server's message : message
// message <K>,<texte> (With K indicating the square where the sound comes from)
int		zappy_message(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "%s\n", __func__);

	if (strstr(client->player.broadcast_msg, "inventaire")) {
		// si on recoit un broadcast inventaire il faut update linventaire de la team
	}
	return (r);
}

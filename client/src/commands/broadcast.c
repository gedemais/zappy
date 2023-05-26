#include "zappy_client.h"


// message <A> texte <texte> (A : case d où provient le son)
int		zappy_broadcast(zappy_client_t *client)
{
	int	r = 0;

	fprintf(stderr, "%s\n", __func__);

	if (strstr(client->player.broadcast_msg, "inventaire")) {
		// si on recoit un broadcast inventaire il faut update linventaire de la team
	}
	return (r);
}

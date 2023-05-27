#include "zappy_client.h"


// handle server's message : message
// message <K>,<texte> (With K indicating the square where the sound comes from)
int		zappy_message(zappy_client_t *client)
{
	int		r = 0;
	uint8_t	id;

	fprintf(stderr, "%s\n", __func__);

	if (strstr((char *)client->buf, "inventaire")) {
		// si on recoit un broadcast inventaire il faut update linventaire de la team
		// il faut donc connaitre l'id du joueur qui emet afin d'update son inventaire pour pouvoir update le team inventaire
		id = atoi(strstr((char *)client->buf, "player_id ")) - 48;
		client->player.inventaire[id] = deserialize_inventaire(client->buf);
		update_team_inventaire(client);
	}
	return (r);
}

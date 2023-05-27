#include "zappy_client.h"


// response: <A>, <B> (A: free slot, B: current slot: ID)
int		zappy_connect_nbr_cb(zappy_client_t *client)
{
	int		r = 0;
	char	tmp = 0;

	// on determine la taille de la team et le nb de joueurs connectes
	// on donne un id au joueur
	if (client->team.size == 0 || client->player.id == 0) {
		// client->team.size = freeslot + currentslot
		// freeslot
		client->team.size = *client->buf - 48;
		memcpy(&tmp, strstr((char *)client->buf, ", ") + 2, sizeof(char));
		// currentslot
		// assignation de l'id
		client->player.id = tmp - 48;
		client->team.nb_player = client->player.id;
		client->team.size += client->player.id;
		fprintf(stderr, "%s: team size: %d, nb_player: %d, id: %d\n----------\n",
			__func__, client->team.size, client->team.nb_player, client->player.id);
	} else {
		// currentslot
		memcpy(&tmp, strstr((char *)client->buf, ", ") + 2, sizeof(char));
		client->team.nb_player = tmp - 48;
	}
	// on ce prepare a le mettre au travail
	client->task = PLAYER_TASK_LOOK;
	return (r);
}

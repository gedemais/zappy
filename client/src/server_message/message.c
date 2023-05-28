#include "zappy_client.h"


static void	message_inventaire(zappy_client_t *client)
{
	uint8_t	id;
	char	*team;
	char	**str_split;
	int		split_len;

	// format : broadcast inventaire {nourriture n, linemate n, deraumere n,
	//		sibur n, mendiane n, phiras n, thystame n, ttl n},player_id n,team_name foo
	//

	str_split = ft_strsplit((char *)client->buf, ",");
	if (str_split == NULL)
		return ;

	fprintf(stderr, "SPLIT ----------\n");
	ft_arr_cprint(str_split);
	fprintf(stderr, "END SPLIT ----------\n");

	// on sait que la derniere case du tableau est le nom de la team
	// l'avant derniere est l'id du joueur
	// split_len = ft_arrlen(str_split);

	team = NULL;

	if (!strcmp(team, client->team.name)) {
		// deserialize_inventaire(client->buf, &client->player.inventaire[id]);
		// update_team_inventaire(client);
	}
}

// handle server's message : message
// message: message <K>,<texte> (With K indicating the square where the sound comes from)
int			zappy_message(zappy_client_t *client)
{
	int		r = 1;

	fprintf(stderr, "%s\n", __func__);

	if (strstr((char *)client->buf, "inventaire")) {
		message_inventaire(client);
	}

	fprintf(stderr, "----------\n");
	return (r);
}

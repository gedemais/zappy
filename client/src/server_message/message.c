#include "zappy_client.h"


// format : broadcast inventaire:{nourriture n, ...}-player_id:n-team_name:foo
static void	message_inventaire(zappy_client_t *client)
{
	uint8_t	id;
	char	*team;
	char	**str_split;
	int		split_len;

	str_split = ft_strsplit((char *)client->buf, "-");
	if (str_split == NULL)
		return ;

	fprintf(stderr, "SPLIT ----------\n");
	ft_arr_cprint(str_split);
	fprintf(stderr, "END SPLIT ----------\n");

	split_len = ft_arrlen(str_split);
	// on sait que la derniere case du tableau est le nom de la team
	team = strstr(str_split[split_len], "team_name:") + strlen("team_name:");
	// l'avant derniere est l'id du joueur
	id = atoi(strstr(str_split[split_len - 1], "player_id:") + strlen("player_id:"));
	fprintf(stderr, "player_id: %d - team_name: %s\n", id, team);
	// le broadcast concerne le joueur car ils sont dans la meme team
	if (!strcmp(team, client->team.name)) {
		// on update l'inventaire du joueur emetteur
		deserialize_inventaire(client->buf, &client->player.inventaire[id]);
		// on update l'inventaire de la team
		update_team_inventaire(client);
	}
	ft_arrfree(str_split);
}

// handle server's message : message
// message: message <K>,<texte> (With K indicating the square where the sound comes from)
int			zappy_message(zappy_client_t *client)
{
	int		r = 0;

	fprintf(stderr, "%s----------\n", __func__);

	if (strstr((char *)client->buf, "inventaire")) {
		message_inventaire(client);
	}

	fprintf(stderr, "----------\n");
	return (r);
}


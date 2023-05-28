#include "zappy_client.h"


void			print_inventaire(t_inventaire inventaire)
{
	fprintf(stderr, "\n"
		"nourriture	: %d\n"
		"linemate	: %d\n"
		"deraumere	: %d\n"
		"sibur		: %d\n"
		"mendiane	: %d\n"
		"phiras		: %d\n"
		"thystame	: %d\n"
		"ttl		: %d\n",
		inventaire.nourriture,
		inventaire.linemate,
		inventaire.deraumere,
		inventaire.sibur,
		inventaire.mendiane,
		inventaire.phiras,
		inventaire.thystame,
		inventaire.ttl);
}

void			update_team_inventaire(zappy_client_t *client)
{
	t_inventaire	inventaire_player;

	// on reset l'inventaire de team
	memset(&client->team.inventaire, 0, sizeof(t_inventaire));
	// on loop sur l'inventaire de tout les joueurs
	for (int i = 0; i < client->team.nb_player + 1; i++) {

		inventaire_player = client->player.inventaire[i];
		client->team.inventaire.nourriture += inventaire_player.nourriture;
		client->team.inventaire.linemate += inventaire_player.linemate;
		client->team.inventaire.deraumere += inventaire_player.deraumere;
		client->team.inventaire.sibur += inventaire_player.sibur;
		client->team.inventaire.mendiane += inventaire_player.mendiane;
		client->team.inventaire.phiras += inventaire_player.phiras;
		client->team.inventaire.thystame += inventaire_player.thystame;
	}
}

void		deserialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE], t_inventaire *inventaire)
{
	char			**str_split;
	char			*str;
	int				tmp;
	uint8_t			i = 0;
	int				*ptr = (int *)inventaire;

	// on split la string pour avoir une string par type de ressources
	str_split = ft_strsplit((char *)inventaire_str, ",");
	if (str_split == NULL)
		return ;
	memset(inventaire, 0, sizeof(t_inventaire));
	// on loop sur toutes les ressources pour les assignation a la structure inventaire
	while (i < R_MAX - 1) {
		// on avance str_split[i] de 1 pour skip l'espace qui precede le nom de la ressource
		// pour i == 0 ce n'est pas un espace mais le { ouvrant
		// on avance le ptr du nom de la ressource + 1 pour l'espace qui suit le nom de la ressource
		str = strstr((char *)str_split[i] + 1, ressources[i].name) + ressources[i].len + 1;
		// on atoi car l'int a recuperer peut etre plus grand que 10
		ptr[i] = atoi(str);
		++i;
	}
	// il reste a gerer le ttl qui n'est pas dans l'enum des ressources
	str = strstr((char *)str_split[i] + 1, "ttl") + strlen("ttl") + 1;
	// on reduit la string de 1 pour supprimer le } fermant
	str[strlen(str) - 1] = '\0';
	ptr[i] = atoi(str);
	// on free le tableau split
	ft_arrfree(str_split);
}

// response: {nourriture n, linemate n, deraumere n, sibur n, mendiane n, phiras n, thystame n, ttl n}
void			serialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE], t_inventaire inventaire)
{
	bzero(inventaire_str, CLIENT_BUFSIZE);
	snprintf((char *)inventaire_str, CLIENT_BUFSIZE, "{"
		"nourriture %d, "
		"linemate %d, "
		"deraumere %d, "
		"sibur %d, "
		"mendiane %d, "
		"phiras %d, "
		"thystame %d, "
		"ttl %d"
		"}",
		inventaire.nourriture,
		inventaire.linemate,
		inventaire.deraumere,
		inventaire.sibur,
		inventaire.mendiane,
		inventaire.phiras,
		inventaire.thystame,
		inventaire.ttl);
}

int				zappy_inventaire_cb(zappy_client_t *client)
{
	int	r = 0;

	// on stock linventaire du joueur
	deserialize_inventaire(client->buf, &client->player.inventaire[client->player.id]);
	// on update son inventaire de team
	update_team_inventaire(client);

	// debug functions for serialize / deserialize --------------------------------------------------
	// print_inventaire(client->player.inventaire[client->player.id]);
	// serialize_inventaire(client->player.broadcast_msg, client->player.inventaire[client->player.id]);
	// fprintf(stderr, "\nserialize inventaire: %s\n----------\n", client->player.broadcast_msg);
	// ----------------------------------------------------------------------------------------------

	client->player.broadcast = BROADCAST_INVENTAIRE;
	client->task = PLAYER_TASK_BROADCAST;
	return (r);
}

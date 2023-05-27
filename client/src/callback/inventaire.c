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
		"thystame	: %d\n\n",
		inventaire.nourriture,
		inventaire.linemate,
		inventaire.deraumere,
		inventaire.sibur,
		inventaire.mendiane,
		inventaire.phiras,
		inventaire.thystame);
}

void			update_team_inventaire(zappy_client_t *client)
{
	t_inventaire	inventaire_player;

	// on reset l'inventaire de team
	memset(&client->team.inventaire, 0, sizeof(t_inventaire));
	// on loop sur l'inventaire de tout les joueurs
	for (unsigned int i = 0; i < PLAYER_MAX + 1; i++) {

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

// TODO : reformater apres correction API
t_inventaire	deserialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE])
{
	t_inventaire	inventaire;

	memset(&inventaire, 0, sizeof(t_inventaire));
	memcpy(&inventaire.nourriture, strstr((char *)inventaire_str, "nourriture") - 2, sizeof(char));
	memcpy(&inventaire.linemate, strstr((char *)inventaire_str, "linemate") - 2, sizeof(char));
	memcpy(&inventaire.deraumere, strstr((char *)inventaire_str, "deraumere") - 2, sizeof(char));
	memcpy(&inventaire.sibur, strstr((char *)inventaire_str, "sibur") - 2, sizeof(char));
	memcpy(&inventaire.mendiane, strstr((char *)inventaire_str, "mendiane") - 2, sizeof(char));
	memcpy(&inventaire.phiras, strstr((char *)inventaire_str, "phiras") - 2, sizeof(char));
	memcpy(&inventaire.thystame, strstr((char *)inventaire_str, "thystame") - 2, sizeof(char));
	inventaire.nourriture -= 48;
	inventaire.linemate -= 48;
	inventaire.deraumere -= 48;
	inventaire.sibur -= 48;
	inventaire.mendiane -= 48;
	inventaire.phiras -= 48;
	inventaire.thystame -= 48;
	return (inventaire);
}

// {9 nourriture, 0 linemate, 0 deraumere, 0 sibur, 0 mendiane, 0 phiras, 0 thystame}
// TODO : reformater apres correction API
void			serialize_inventaire(uint8_t inventaire_str[CLIENT_BUFSIZE], t_inventaire inventaire)
{
	bzero(inventaire_str, CLIENT_BUFSIZE);
	snprintf((char *)inventaire_str, CLIENT_BUFSIZE, "{"
		"%d nourriture, "
		"%d linemate, "
		"%d deraumere, "
		"%d sibur, "
		"%d mendiane, "
		"%d phiras, "
		"%d thystame"
		"}",
		inventaire.nourriture,
		inventaire.linemate,
		inventaire.deraumere,
		inventaire.sibur,
		inventaire.mendiane,
		inventaire.phiras,
		inventaire.thystame);
}

int				zappy_inventaire_cb(zappy_client_t *client)
{
	int	r = 0;

	// on stock linventaire du joueur
	client->player.inventaire[client->player.id] = deserialize_inventaire(client->buf);
	// on update son inventaire de team
	update_team_inventaire(client);

	// debug functions for serialize / deserialize --------------------------------------------------
	// print_inventaire(client->player.inventaire[client->player.id]);
	// serialize_inventaire(client->player.broadcast_msg, client->player.inventaire[client->player.id]);
	// fprintf(stderr, "\nbroadcast msg : %s\n", client->player.broadcast_msg);
	// ----------------------------------------------------------------------------------------------

	// on ce prepare a le broadcast aux autres joueurs pour qu'ils actualisent leurs inventaires de team
	snprintf(client->player.broadcast_msg, CLIENT_BUFSIZE,
		"%s %s player_id %d", commands[CMD_INVENTAIRE].name, client->buf, client->player.id);
	client->task = PLAYER_TASK_BROADCAST;
	return (r);
}
